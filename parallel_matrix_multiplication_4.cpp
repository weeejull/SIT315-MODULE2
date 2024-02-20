// NAME = VIJUL
// ROLL NUMBER = 2210994860
// MODULE 2 - CONCURRENT PROGRAMMING
// TASK M2T1P : PARALLEL MATRIX MULTIPLICATION

// INCLUDING IMPORTANT STANDARD LIBRARIES OF C++
#include <iostream>  // INPUT OUTPUT STREAM
#include <ctime>     // TIME BASED GENERATION
#include <cstdlib>   // LIBRARY TO EXECUTE C FUNCTIONS
#include <fstream>   // INPUT OUTPUT OPERATIONS
#include <chrono> 
#include <pthread.h>

using namespace std; // ENCAPSULATING THE C++ STANDARD LIBRARIES
using namespace std::chrono;

struct ThreadData {
    int thread_id;
    int num_threads;
    int **A;
    int **B;
    int **C;
    int N;
};

// FUNCTION TO PERFORM MATRIX MULTIPLICATION IN PARALLEL
void *MULTIPLY_MATRICES_PARALLEL(void *thread_data) {
    ThreadData *data = static_cast<ThreadData *>(thread_data);
    int start_row = (data->thread_id * data->N) / data->num_threads;
    int end_row = ((data->thread_id + 1) * data->N) / data->num_threads;

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < data->N; ++j) {
            data->C[i][j] = 0;
            for (int k = 0; k < data->N; ++k) {
                data->C[i][j] += data->A[i][k] * data->B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

// FUNCTION TO GENERATE A RANDOM MATRIX
void GENERATE_RANDOM_MATRIX(int **matrix, int N) 
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            matrix[i][j] = rand() % 10;  
        }
    }
}

// FUNCTION TO PERFORM MATRIX MULTIPLICATION SEQUENTIALLY
void MULTIPLY_MATRICES(int **A, int **B, int **C, int N) 
{
    // ITERATE OVER THE ROWS OF MATRIX A
    for (int i = 0; i < N; ++i) 
    {
        // ITERATE OVER THE COLUMNS OF MATRIX B
        for (int j = 0; j < N; ++j) 
        {
            C[i][j] = 0;  
            // MULTIPLY AND ACCUMULATE ELEMENTS
            for (int k = 0; k < N; ++k) 
            {
                C[i][j] += A[i][k] * B[k][j];  
            }
        }
    }
}

// FUNCTION TO PRINT THE MATRIX TO THE TERMINAL
void PRINT_MATRIX_TO_TERMINAL(int **matrix, int N) 
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            cout << matrix[i][j] << " ";  // PRINT ELEMENT FOLLOWED BY A BLANK SPACE
        }
        cout << endl;  // MOVE TO THE NEXT LINE AFTER EACH ROW
    }
}

// MAIN FUNCTION OF THE PROGRAM
int MAIN() {
    // VARYING MATRIX SIZES
    for (int MATRIX_SIZE = 100; MATRIX_SIZE <= 1000; MATRIX_SIZE += 100) {
        // VARYING NUMBER OF THREADS
        for (int NUM_THREADS = 2; NUM_THREADS <= 8; NUM_THREADS *= 2) {
            // ALLOCATING MEMORY FOR EACH MATRIX BY DYNAMIC MEMORY ALLOCATION
            int **A = new int*[MATRIX_SIZE];
            int **B = new int*[MATRIX_SIZE];
            int **C = new int*[MATRIX_SIZE];

            for (int i = 0; i < MATRIX_SIZE; ++i) {
                A[i] = new int[MATRIX_SIZE];
                B[i] = new int[MATRIX_SIZE];
                C[i] = new int[MATRIX_SIZE];
            }

            srand(time(0));

            // GENERATING RANDOM MATRICES A AND B
            GENERATE_RANDOM_MATRIX(A, MATRIX_SIZE);
            GENERATE_RANDOM_MATRIX(B, MATRIX_SIZE);

            auto START_TIME = high_resolution_clock::now();

            // Create an array of pthread_t to hold thread IDs
            pthread_t THREADS[NUM_THREADS];
            // Create an array of ThreadData to hold parameters for each thread
            ThreadData THREAD_DATA[NUM_THREADS];

            for (int i = 0; i < NUM_THREADS; ++i) {
                THREAD_DATA[i].thread_id = i;
                THREAD_DATA[i].num_threads = NUM_THREADS;
                THREAD_DATA[i].A = A;
                THREAD_DATA[i].B = B;
                THREAD_DATA[i].C = C;
                THREAD_DATA[i].N = MATRIX_SIZE;

                pthread_create(&THREADS[i], NULL, MULTIPLY_MATRICES_PARALLEL, (void *)&THREAD_DATA[i]);
            }

            // JOINING ALL THE THREADS
            for (int i = 0; i < NUM_THREADS; ++i) {
                pthread_join(THREADS[i], NULL);
            }

            // PERFORMING MATRIX MULTIPLICATION
            MULTIPLY_MATRICES(A, B, C, MATRIX_SIZE);

            auto END_TIME = high_resolution_clock::now();
            auto EXECUTION_TIME = duration_cast<microseconds>(END_TIME - START_TIME);

            // Print the matrix size, thread count, and execution time
            cout << "MATRIX SIZE: " << MATRIX_SIZE << "x" << MATRIX_SIZE << ", ";
            cout << "THREAD COUNT: " << NUM_THREADS << ", ";
            cout << "EXECUTION TIME: " << EXECUTION_TIME.count() << " MICROSECONDS" << endl;

            // DEALLOCATING MEMORY
            for (int i = 0; i < MATRIX_SIZE; ++i) {
                delete[] A[i];
                delete[] B[i];
                delete[] C[i];
            }

            delete[] A;
            delete[] B;
            delete[] C;
        }
    }

    return 0;
}
