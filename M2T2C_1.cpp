// NAME = VIJUL
// ROLL NUMBER = 2210994860
// MODULE 2 - CONCURRENT PROGRAMMING
// TASK M2T2C - COMPLEX THREADING
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

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
void quickSort(int arr[], int start, int end) {
    if (start >= end)
        return;

    // THS LINE PARTITIONS THE ARRAY
    int p = partition(arr, start, end);
    // THIS LINE SORTS THE LEFT PART OF THE ARRAY
    quickSort(arr, start, p - 1);
    // THIS LINE SORTS THE RIGHT PART OF THE ARRAY
    quickSort(arr, p + 1, end);
}

int main() {
    int arr[] = { 9, 3, 4, 2, 1, 8, 12, 15, 14, 50, 48, 57, 55, 47, 22, 43 };
    int n = 16;

    // MEASURING THE STARTING TIME IN MICROSECONDS
    auto start_time = high_resolution_clock::now();

    // PERFORMING QUICKSORT ALGORITHM
    quickSort(arr, 0, n - 1);

    // MEASURING THE END TIME IN MICROSECONDS
    auto end_time = high_resolution_clock::now();

    // CALCULATING THE ELAPSED TIME
    auto duration = duration_cast<microseconds>(end_time - start_time);

    // PRINTING THE SORTED ARRAY
    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    // PRINTING THE EXECUTION TIME
    cout << "\nExecution time: " << duration.count() << " microseconds" << endl;

    return 0;
}
