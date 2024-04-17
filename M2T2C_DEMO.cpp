#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

// FUNCTION TO PARTITION THE ARRAY AND RETURN THE PIVOT INDEX
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// QUICKSORT FUNCTION
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    // ARRAY SIZES TO BE TESTED
    vector<int> arr_sizes = {10000, 100000, 10000000};
    for (int size : arr_sizes) {
        vector<int> arr(size);
        // FILLING THE ARRAY WITH RANDOM VALUES
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % size;
        }

        auto start_time = high_resolution_clock::now();
        // MEASURING EXECUTION TIME OF QUICKSORT
        quickSort(arr, 0, size - 1);
        auto end_time = high_resolution_clock::now();

        // CALCULATING DURATION
        auto duration = duration_cast<milliseconds>(end_time - start_time);
        // PRINTING ARRAY SIZE AND EXECUTION TIME
        cout << "Array size: " << size << ", Execution time: " << duration.count() << " milliseconds" << endl;
    }
    return 0;
}
