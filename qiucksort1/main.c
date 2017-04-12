#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 20000
#define X 1000

void quicksort(int *, int, int);
int partition(int *, int, int);
int choosePivot(int *, int, int);
void swap(int *, int *);

int main(int argc, char **argv)
{
  int rank, size, pivot, partner, recvSize;
  double start, end;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int * newArray;
  int * array = (int *) malloc(sizeof(int)*N/size);
  int * recvBuffer = (int *) malloc(sizeof(int)*N/size);

  int i, j;
  for(i = 0; i < N/size; i++)
    array[i] = rand()%X;
  if(rank == 0)
  {
    start = MPI_Wtime();
    pivot = choosePivot(arr, 0, N/size-1);
    printf("The pivot is %d\n", pivot);
  }
  MPI_Bcast(&pivot, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //Assume that the number of processes is a power of 2
  int storeIdx = 0;
  int arrSize = N/size;

  for(partner = size/2; partner > 0; partner = partner >> 1)
  {
    storeIdx = 0;
    for(i = 0; i < arraySize; i++)
    {
      if(array[i] < pivot)
      {
        swap(&array[i], &array[storeIdx]);
        storeIdx++;
      }
    }
    printf("storeIdx = %d in process %d\n", storeIdx, rank);
    int flag = 0;
    MPI_Request request, requestSend;
    if( (rank / partner) % 2 == 0)
    {
        printf("rank + partner is %d\n", rank + partner);
        int sendVal = arraySize-storeIdx;
        recvSize = 0;
        MPI_Isend(&sendVal, 1, MPI_INT, rank+partner, partner+size, MPI_COMM_WORLD, &requestSend);
        MPI_Irecv(&recvSize, 1, MPI_INT, rank+partner, partner+size, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
    }
    else
    {
        printf("rank - size is %d\n", rank - partner);
        int sendVal = storeIdx;
        recvSize = 0;
        MPI_Isend(&sendVal, 1, MPI_INT, rank-partner, partner+size, MPI_COMM_WORLD, &requestSend);
        MPI_Irecv(&recvSize, 1, MPI_INT, rank-partner, partner+size, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(recvSize > 0)
    {
       //Merge arrays
      if((rank / partner) % 2 == 0) //Keep smaller elements
      {
        newArray = (int *) malloc(sizeof(int)*(recvSize+storeIdx));
        for(i = 0; i < storeIdx; i++)
          newArray[i] = array[i];
        for(j = 0, i = storeIdx; i < recvSize+storeIdx; i++, j++)
          newArray[i] = recvBuffer[j];
        free((void *) arr);
        array = newArray;
        newArray = NULL;
        arraySize = recvSize+storeIdx;
      }
      else  //Keep larger elements
      {
        newArray = (int *) malloc(sizeof(int)*(recvSize+(arraySize-storeIdx)));
        for(j = 0, i = storeIdx; i < arraySize; i++, j++)
          newArray[j] = array[i];
        for(j = 0, i = arraySize-storeIdx; i < recvSize+(arraySize-storeIdx); i++, j++)
          newArray[i] = recvBuffer[j];
        free((void *) arr);
        array = newArray;
        newArray = NULL;
        arraySize = recvSize+(arraySize-storeIdx);
      }
      else
      {
      arraySize = 0;
      }
      if(rank % partner == 0)
      {
        pivot = choosePivot(arr, 0, arraySize-1);
        for(i = 1; i < partner; i++)
            MPI_Send(&pivot, 1, MPI_INT, rank+i, partner+1, MPI_COMM_WORLD);
      }
      else
      {
            MPI_Recv(&pivot, 1, MPI_INT, partner*(rank/partner), partner+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }
    if(arraySize > 0)
        quicksort(arr, 0, arrSize-1);
    int * sizeArray, * fullArray, * displacement;
    if(rank == 0)
    {
        sizeArray = (int *) malloc(sizeof(int)*size);
        fullArray = (int *) malloc(sizeof(int)*N);
    }
  }
  MPI_Finalize();
  return 0;
}

void quicksort(int * array, int low, int hight)
{
  if(low < hight)
  {
    int p = partition(array, low, hight);
    quicksort(array, low, p - 1);
    quicksort(array, p + 1, hight);
  }
}
int partition(int * array, int low, int hight)
{
  int i;
  int pivotIdx = choosePivot(array, low, hight);
  int pivotVal = array[pivotIdx];

  swap(&array[pivotIdx], &array[hight]);

  int storeIdx = low;
  for(i = low; i < hight; i++)
  {
    if(array[i] < pivotVal)
    {
      swap(&array[i], &array[storeIdx]);
      storeIdx++;
    }
  }
  swap(&array[storeIdx], &array[hight]);
  return storeIdx;
}
void swap(int * x, int * y)
{
  int temp = *x;
  *x = *y;
  *y = temp;
}
//Select the median of array[low], array[hight], and array[(low+hight)/2]
int choosePivot(int * array, int low, int hight)
{
  int mid = (low+hight)/2;
  int temp;
  if(array[low] > array[hight])
  {
    temp = low;
    low = hight;
    hight = temp;
  }
  if(array[mid] < array[low])
  {
    temp = mid;
    mid = low;
    low = temp;
  }
  if(array[hight] < array[mid])
  {
    temp = mid;
    mid = hight;
    hight = temp;
  }
  return array[mid];
}
