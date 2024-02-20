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
#include <omp.h>

using namespace std; // ENCAPSULATING THE C++ STANDARD LIBRARIES
using namespace std::chrono;

// DECLARATION OF THREAD DATA STRUCTURE
struct ThreadData {
    int thread_id;
    int num_threads;
    int **A;
    int **B;
    int **C;
    int N;
};

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

// FUNCTION TO PERFORM MATRIX MULTIPLICATION IN PARALLEL USING PTHREAD
void *MULTIPLY_MATRICES_PARALLEL_PTHREAD(void *thread_data) {
    // CASTING THE VOID POINTER TO THREAD DATA
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

// FUNCTION TO PERFORM MATRIX MULTIPLICATION IN PARALLEL USING OPENMP
void MULTIPLY_MATRICES_PARALLEL_OPENMP(int **A, int **B, int **C, int N) 
{
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
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
int main() {
    // VARYING MATRIX SIZES
    for (int MATRIX_SIZE = 100; MATRIX_SIZE <= 1000; MATRIX_SIZE += 100) {
        // VARYING NUMBER OF THREADS
        for (int NUM_THREADS = 2; NUM_THREADS <= 8; NUM_THREADS *= 2) {
            // ALLOCATING MEMORY FOR EACH MATRIX BY DYNAMIC MEMORY ALLOCATION
            int **A = new int*[MATRIX_SIZE];
            int **B = new int*[MATRIX_SIZE];
            int **C_PTHREAD = new int*[MATRIX_SIZE];
            int **C_OPENMP = new int*[MATRIX_SIZE];
            int **C_SEQ = new int*[MATRIX_SIZE];

            for (int i = 0; i < MATRIX_SIZE; ++i) {
                A[i] = new int[MATRIX_SIZE];
                B[i] = new int[MATRIX_SIZE];
                C_PTHREAD[i] = new int[MATRIX_SIZE];
                C_OPENMP[i] = new int[MATRIX_SIZE];
                C_SEQ[i] = new int[MATRIX_SIZE];
            }

            srand(time(0));

            // GENERATING RANDOM MATRICES A AND B
            GENERATE_RANDOM_MATRIX(A, MATRIX_SIZE);
            GENERATE_RANDOM_MATRIX(B, MATRIX_SIZE);

            auto START_TIME_SEQ = high_resolution_clock::now();
            // PERFORMING MATRIX MULTIPLICATION SEQUENTIALLY
            MULTIPLY_MATRICES(A, B, C_SEQ, MATRIX_SIZE);
            auto END_TIME_SEQ = high_resolution_clock::now();
            auto EXECUTION_TIME_SEQ = duration_cast<microseconds>(END_TIME_SEQ - START_TIME_SEQ);

            auto START_TIME_PTHREAD = high_resolution_clock::now();
            // PERFORMING MATRIX MULTIPLICATION IN PARALLEL USING PTHREAD
            // Create an array of pthread_t to hold thread IDs
            pthread_t THREADS_PTHREAD[NUM_THREADS];
            // Create an array of ThreadData to hold parameters for each thread
            ThreadData THREAD_DATA_PTHREAD[NUM_THREADS];

            for (int i = 0; i < NUM_THREADS; ++i) {
                THREAD_DATA_PTHREAD[i].thread_id = i;
                THREAD_DATA_PTHREAD[i].num_threads = NUM_THREADS;
                THREAD_DATA_PTHREAD[i].A = A;
                THREAD_DATA_PTHREAD[i].B = B;
                THREAD_DATA_PTHREAD[i].C = C_PTHREAD;
                THREAD_DATA_PTHREAD[i].N = MATRIX_SIZE;

                pthread_create(&THREADS_PTHREAD[i], NULL, MULTIPLY_MATRICES_PARALLEL_PTHREAD, (void *)&THREAD_DATA_PTHREAD[i]);
            }

            // JOINING ALL THE THREADS
            for (int i = 0; i < NUM_THREADS; ++i) {
                pthread_join(THREADS_PTHREAD[i], NULL);
            }
            auto END_TIME_PTHREAD = high_resolution_clock::now();
            auto EXECUTION_TIME_PTHREAD = duration_cast<microseconds>(END_TIME_PTHREAD - START_TIME_PTHREAD);

            auto START_TIME_OPENMP = high_resolution_clock::now();
            // PERFORMING MATRIX MULTIPLICATION IN PARALLEL USING OPENMP
            MULTIPLY_MATRICES_PARALLEL_OPENMP(A, B, C_OPENMP, MATRIX_SIZE);
            auto END_TIME_OPENMP = high_resolution_clock::now();
            auto EXECUTION_TIME_OPENMP = duration_cast<microseconds>(END_TIME_OPENMP - START_TIME_OPENMP);

            // Print the matrix size, thread count, and execution time
            cout << "MATRIX SIZE: " << MATRIX_SIZE << "x" << MATRIX_SIZE << ", ";
            cout << "THREAD COUNT: " << NUM_THREADS << ", ";
            cout << "SEQUENTIAL EXECUTION TIME: " << EXECUTION_TIME_SEQ.count() << " MICROSECONDS, ";
            cout << "PTHREAD EXECUTION TIME: " << EXECUTION_TIME_PTHREAD.count() << " MICROSECONDS, ";
            cout << "OPENMP EXECUTION TIME: " << EXECUTION_TIME_OPENMP.count() << " MICROSECONDS" << endl;

            // DEALLOCATING MEMORY
            for (int i = 0; i < MATRIX_SIZE; ++i) {
                delete[] A[i];
                delete[] B[i];
                delete[] C_PTHREAD[i];
                delete[] C_OPENMP[i];
                delete[] C_SEQ[i];
            }

            delete[] A;
            delete[] B;
            delete[] C_PTHREAD;
            delete[] C_OPENMP;
            delete[] C_SEQ;
        }
    }

    return 0;
}
