//
// Created by Darius Andzevičius on 2020-05-25.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "/usr/local/Cellar/open-mpi/4.0.3/include/mpi.h"

void insertionSort(int* arr, int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void printArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int N = 100000;
    const int M = N / 10; // max value in array
    clock_t clok;

    MPI_Init(NULL, NULL);

    int worldSize; // Procesu skaicius
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int worldRank; // Proceso id
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    char procName[MPI_MAX_PROCESSOR_NAME];
    int nameLen; // Proceso vardas
    MPI_Get_processor_name(procName, &nameLen);

    int* array = NULL;
    int* sorted = NULL;

    if (worldRank == 0) {
        array = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            array[i] = (rand() % M) + 1;
        }
        clok = clock();
    }

    int sizeToProcess = N / worldSize;
    int* subArray = malloc(sizeToProcess * sizeof(int));
    MPI_Scatter(array, sizeToProcess, MPI_INT, subArray, sizeToProcess, MPI_INT, 0, MPI_COMM_WORLD);

    insertionSort(subArray, sizeToProcess);

    if (worldRank == 0) {
        sorted = malloc(N * sizeof(int));
    }

    MPI_Gather(subArray, sizeToProcess, MPI_INT, sorted, sizeToProcess, MPI_INT, 0, MPI_COMM_WORLD);

    if (worldRank == 0) {
        insertionSort(sorted, N);
    }

    if (worldRank == 0) {
        clok = clock() - clok;
        double time_taken = ((double) clok) / CLOCKS_PER_SEC; // in seconds
        printf("Programa užtruko: %f sekundes su %d branduoliu \n", time_taken, worldSize);
    }

    free(sorted);
    free(subArray);
    free(array);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}