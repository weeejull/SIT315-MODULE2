// NAME = VIJUL
// ROLL NUMBER = 2210994860
// MODULE 2
// SEMINAR 5 – MULTITHREADING – OMP
// ACTIVITY 1 – PARALLEL VECTOR ADDITION – OMP

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <omp.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
    // GENERATING RANDOM VALUES FOR THE VECTOR
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    unsigned long size = 100000000;

    srand(time(0));

    int *v1, *v2, *v3;

    auto start = high_resolution_clock::now();

    // DYNAMIC MEMORY ALLOCATION
    v1 = (int *)malloc(size * sizeof(int));
    v2 = (int *)malloc(size * sizeof(int));
    v3 = (int *)malloc(size * sizeof(int));

    // FILLING THE VECTORS WITH RANDOMN VALUE
    randomVector(v1, size);
    randomVector(v2, size);

    int total = 0; // THIS VARIABLE STORES THE TOTAL SUM

    // PARALLELIZED VECTOR ADDITION USING OPENMP
    // ADDED THE CRITICAL SECTION FOR UPDATING THE TOTAL SUM
    #pragma omp parallel default(none) shared(v1, v2, v3, size) private(total)
    {
        int privateTotal = 0; // Private variable for each thread

        // PARALLELIZATION OF VECTOR ADDITION USING OPENMP
        #pragma omp for schedule(static)
        for (int i = 0; i < size; i++) {
            // ADDITION OF VECTOR V1 AND V2 WHICH IS STORED AS A SUM IN V3
            v3[i] = v1[i] + v2[i];
            // PRIVATE TOTAL FOR EACH THREAD
            privateTotal += v3[i];
        }

        // CRITICAL SECTION TO UPDATE THE TOTAL SUM
        #pragma omp critical
        total += privateTotal;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // PRINTING TOTAL SUM
    cout << "Total sum: " << total << endl;
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    // FREE DYNAMICALLY ALLOCATED MEMORY
    free(v1);
    free(v2);
    free(v3);

    return 0;
}
