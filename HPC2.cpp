#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

// Function to generate an array of random integers
void generateRandomArray(int arr[], int size) {
    srand(time(nullptr));
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 100; // Generate random numbers between 0 and 99
    }
}

// Function to print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// Sequential Bubble Sort
void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(int arr[], int size) {
    int i, j;
    #pragma omp parallel for private(i, j)
    for (i = 0; i < size - 1; ++i) {
        for (j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Merge Sort Helper Function: Merge two sorted halves of arr[]
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main() {
    const int SIZE = 19999;
    int arr[SIZE];

    // Generate random array
    generateRandomArray(arr, SIZE);
    // Sequential Bubble Sort
    clock_t start_time = clock();
    bubbleSort(arr, SIZE);
    clock_t end_time = clock();
    std::cout << "Sequential Bubble Sort Time: " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl;

    // Parallel Bubble Sort
    generateRandomArray(arr, SIZE); // Reset array
    start_time = clock();
    parallelBubbleSort(arr, SIZE);
    end_time = clock();
    std::cout << "Parallel Bubble Sort Time: " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl;

    // Generate random array
    generateRandomArray(arr, SIZE);

    // Sequential Merge Sort
    start_time = clock();
    mergeSort(arr, 0, SIZE - 1);
    end_time = clock();
    std::cout << "Sequential Merge Sort Time: " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl;

    // Parallel Merge Sort
    generateRandomArray(arr, SIZE); // Reset array
    start_time = clock();
    parallelMergeSort(arr, 0, SIZE - 1);
    end_time = clock();
    printArray(arr,SIZE);
    std::cout << "Parallel Merge Sort Time: " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl;

    return 0;
}
