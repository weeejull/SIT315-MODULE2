// NAME = VIJUL
// ROLL NUMBER = 2210994860
// MODULE 2 - CONCURRENT PROGRAMMING
// TASK M2T2C - COMPLEX THREADING
#include <iostream>
#include <chrono>
#include <pthread.h> // I AM CHOSING PTHREAD LIBRARY

using namespace std;
using namespace chrono;

// STRUCT TO PASS DATA TO THE THREADS
struct ThreadData
{
int* arr;
int start;
int end;
};

// THIS FUNCTION PARTITIONS THE ENTIRE ARRAY AND IDENTIFIES THE PIVOT INDEX AND RETURNS THE PIVOT INDEX
int partition(int arr[], int start, int end) {
    int pivot = arr[start];
    int count = 0;

    // THIS FUNCTION COUNTS THE ELEMENTS SMALLER THAN OR LARGER THAN THE PIVOT ELEMENT
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }

    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    int i = start, j = end;

    // THIS LOOP SORTS THE LEFT AND RIGHT PARTS OF THE PIVOT ELEMENT
    while (i < pivotIndex && j > pivotIndex) {
        while (arr[i] <= pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}


// THIS FUNCTION PERFORMS THE QUICKSORT ALGORITHM ON THE ARRAY
void* quickSortThreaded(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    int start = data->start;
    int end = data->end;
    int* arr = data->arr;

    if (start >= end) {
        delete data;
        pthread_exit(NULL);
    }

    int p = partition(arr, start, end);

    // THIS LINE CREATES THREADS FOR THE LEFT AND RIGHT PARTITIONS
    pthread_t leftThread, rightThread;
    ThreadData* leftData = new ThreadData{ arr, start, p - 1 };
    ThreadData* rightData = new ThreadData{ arr, p + 1, end };

    pthread_create(&leftThread, NULL, quickSortThreaded, static_cast<void*>(leftData));
    pthread_create(&rightThread, NULL, quickSortThreaded, static_cast<void*>(rightData));

    // THESE LINES WAIT FOR BOTH THE THREADS TO FINISH
    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);

    delete data;  // THE ALLOCATED MEMORY IS FREED FOR THE CURRENT DATA THREAD
    pthread_exit(NULL);
}

int main() {
    int arr[] = { 9, 3, 4, 2, 1, 8, 12, 15, 14, 50, 48, 57, 55, 47, 22, 43 };
    int n = 16;

    // MEASURING THE STARTING TIME IN MICROSECONDS
    auto start_time = high_resolution_clock::now();

    // PREPARING THE DATA FOR THE MAIN THREAD
    ThreadData* mainData = new ThreadData{ arr, 0, n - 1 };

    // CREATING A THREAD FOR THE MAIN SORTING PURPOSE
    pthread_t mainThread;
    pthread_create(&mainThread, NULL, quickSortThreaded, static_cast<void*>(mainData));

    // WAITING FOR THE MAIN THREAD TO FINISH
    pthread_join(mainThread, NULL);

    // MEASURING THE ENDING TIME IN MICROSECONDS
    auto end_time = high_resolution_clock::now();

    // CALCULATING THE ELAPSED TIME IN MICROSECONDS
    auto duration = duration_cast<microseconds>(end_time - start_time);

    // PRINTING THE SORTED ARRAY
    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    // PRINTING THE EXECUTION TIME IN MICROSECONDS
    cout << "\nExecution time: " << duration.count() << " microseconds" << endl;

    return 0;
}
