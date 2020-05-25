#include <iostream>
#include <mpi.h>
//#include "/usr/local/Cellar/open-mpi/4.0.3/include/mpi.h"

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
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

bool isArraySorted(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i+1]) {
            printArray(arr, n);
            return false;
        }
    }
    return true;
}

int* generateArray(int n, int m) {
    int* array = new int[n];
    for (int i = 0; i < n; i++) {
        array[i] = (rand() % m) + 1;
    }
    return array;
}

int main() {
    const int N = 40; // data size
    const int M = N / 10; // max value in array

    MPI_Init(NULL, NULL);

    int worldSize = 5; // Procesu skaicius
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int worldRank = 0; // Proceso id
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    char procName[MPI_MAX_PROCESSOR_NAME];
    int nameLen; // Proceso vardas
    MPI_Get_processor_name(procName, &nameLen);

    MPI_Status status;

    int arrSizeToProccess = N / worldSize;

    int* array;

    MPI_Barrier(MPI_COMM_WORLD);

    std::cout << worldSize << " number of processes" << std::endl;
    if (worldRank == 0) {
        array = generateArray(N, M);
        for (int i = 0; i < worldSize - 1; i++) {
            int lwr = arrSizeToProccess * i;
            int* subArray = &array[lwr];
          MPI_Send(&array[lwr], arrSizeToProccess, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        int *buf;
        MPI_Recv(buf, arrSizeToProccess, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::cout << "Process " << worldRank << " received " << buf << std::endl;
        insertionSort(buf, arrSizeToProccess);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (worldRank == 0) {
        insertionSort(array, N);
        isArraySorted(array, N);
    }

    MPI_Finalize();
    return 0;
}