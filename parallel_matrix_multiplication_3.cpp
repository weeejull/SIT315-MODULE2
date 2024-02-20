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

void *multiplyMatricesParallel(void *thread_data) {
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

// THIS FUNCTION GENERATES A RANDOM MATRIX USING NESTED LOOPS
// THIS NESTED LOOP INSERTS A RANDOM NUMBER FROM 0 TO 99 IN EACH ELEMENT 
void generateRandomMatrix(int **matrix, int N) 
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            matrix[i][j] = rand() % 10;  
        }
    }
}

// THIS FUNCTION PERFORMS MATRIX MULTIPLICATION USING TRIPLE NESTED LOOPS
void multiplyMatrices(int **A, int **B, int **C, int N) 
{
    // THIS FOR LOOP ITERATES OVER THE ROWS OF MATRIX A
    for (int i = 0; i < N; ++i) 
    {
        // THIS FOR LOOP ITERATES OVER THE COLUMNS OF MATRIX B
        for (int j = 0; j < N; ++j) 
        {
            C[i][j] = 0;  
            // THIS FOR LOOP DOES THE MULTIPLICATION AND N IS THE DIMENSION OF THE MATRIX
            for (int k = 0; k < N; ++k) 
            {
                C[i][j] += A[i][k] * B[k][j];  
            }
        }
    }
}

// THIS FUNCTION PRINTS THE MATRIX TO THE TERMINAL USING A NESTED LOOP TO ITERATE OVER EACH ELEMENT OF THE MATRIX
void printMatrixToTerminal(int **matrix, int N) 
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            cout << matrix[i][j] << " ";  // PRINTING EACH ELEMENT FOLLOWED BY A BLANK SPACE
        }
        cout << endl;  // THIS ALLOWS TO MOVE TO THE NEXT LINE AFTER EACH ROW
    }
}

// THE MAIN FUNCTION OF MY PROGRAM
int main() {
    // THE SIZE OF THE MATRIX (N X N)
    int N = 10;
    
    int num_threads = 2;

    // ALLOCATING MEMORY FOR EACH MATRIX BY DYNAMIC MEMORY ALLOCATION
    int **A = new int*[N];
    int **B = new int*[N];
    int **C = new int*[N];

    // ALLOCATING MEMORY FOR EACH ROW IN MATRIX A, B, AND C
    for (int i = 0; i < N; ++i) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    // INITIALIZING THE INTERNAL STATE OF THE RANDOM NUMBER GENERATOR WITH AN INITIAL VALUE
    srand(time(0));

    // GENERATING RANDOM MATRICES A AND B
    generateRandomMatrix(A, N);
    generateRandomMatrix(B, N);

    auto start_time = high_resolution_clock::now();
    
    // Create an array of pthread_t to hold thread IDs
    pthread_t threads[num_threads];
    // Create an array of ThreadData to hold parameters for each thread
    ThreadData thread_data[num_threads];
    
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;
        thread_data[i].N = N;

        pthread_create(&threads[i], NULL, multiplyMatricesParallel, (void *)&thread_data[i]);
    }

    // JOINING ALL THE THREADS
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // PERFORMING MATRIX MULTIPLICATION
    multiplyMatrices(A, B, C, N);

    auto end_time = high_resolution_clock::now();
    auto execution_time = duration_cast<microseconds>(end_time - start_time);

    // PRINTING MATRICES TO THE TERMINAL
    cout << "Matrix A:" << endl;
    printMatrixToTerminal(A, N);

    cout << "\nMatrix B:" << endl;
    printMatrixToTerminal(B, N);

    cout << "\nResult Matrix C:" << endl;
    printMatrixToTerminal(C, N);

    cout << "\nExecution Time: " << execution_time.count() << " milliseconds" << endl;

    // DEALLOCATING MEMORY
    for (int i = 0; i < N; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
