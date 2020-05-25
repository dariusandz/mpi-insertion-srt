//
// Created by Darius Andzevičius on 2020-05-26.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "/usr/local/Cellar/open-mpi/4.0.3/include/mpi.h"

void merge(int *a, int *b, int l, int m, int r) {

    int h, i, j, k;
    h = l;
    i = l;
    j = m + 1;

    while ((h <= m) && (j <= r)) {
        if (a[h] <= a[j]) {
            b[i] = a[h];
            h++;
        } else {
            b[i] = a[j];
            j++;
        }
        i++;
    }

    if (m < h) {
        for (k = j; k <= r; k++) {
            b[i] = a[k];
            i++;
        }
    } else {
        for (k = h; k <= m; k++) {
            b[i] = a[k];
            i++;
        }
    }
    for (k = l; k <= r; k++) {
        a[k] = b[k];
    }
}

void mergeSort(int *a, int *b, int left, int right) {
    if (left < right) {
        int middle = (left + right) / 2;
        mergeSort(a, b, left, middle);
        mergeSort(a, b, (middle + 1), right);
        merge(a, b, left, middle, right);
    }
}

void mergeWithDepth(int *a, int *b, int left, int right, int depth) {
    if (left < right && depth > 1) {
        int middle = (left + right) / 2;
        mergeWithDepth(a, b, left, middle, depth / 2);
        mergeWithDepth(a, b, (middle + 1), right, depth / 2);
        merge(a, b, left, middle, right);
    }
}

bool isArraySorted(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i+1]){
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv) {
    const int N = atoi(argv[1]);
    const int M = N / 10; // max value in array
    clock_t clok;

    MPI_Init(NULL, NULL);

    int worldSize; // Procesu skaicius
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int worldRank; // Proceso id
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    int* array;
    if (worldRank == 0) {
        array = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            array[i] = (rand() % M) + 1;
        }
        clok = clock();
    }

    int arrSizeToProccess = N / worldSize;
    int* subArray = malloc(arrSizeToProccess * sizeof(int));
    MPI_Scatter(array, arrSizeToProccess, MPI_INT, subArray, arrSizeToProccess, MPI_INT, 0, MPI_COMM_WORLD);

    int* tempArray = malloc(arrSizeToProccess * sizeof(int));
    mergeSort(subArray, tempArray, 0, (arrSizeToProccess - 1));

    int* sorted;
    if (worldRank == 0) {
        sorted = malloc(N * sizeof(int));
    }

    MPI_Gather(subArray, arrSizeToProccess, MPI_INT, sorted, arrSizeToProccess, MPI_INT, 0, MPI_COMM_WORLD);

    if (worldRank == 0) {
        int* arr = malloc(N * sizeof(int));
        mergeWithDepth(sorted, arr, 0, N - 1, worldSize);
        printf("array size: %d, processors:  %d, sorted in %f", N, worldSize, ((double) clok) / CLOCKS_PER_SEC);
        free(sorted);
        free(arr);
    }

    free(array);
    free(tempArray);
    free(subArray);

    MPI_Finalize();
}
