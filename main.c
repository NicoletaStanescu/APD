#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define M 3
#define K 2
#define N 3

struct v
{
    int i; //row
    int j; //column
};

int A[M][K] = {{1,4},{2,5},{3,6}};
int B[K][N] = {{8,7,6},{5,4,3}};
int C[M][N];

void workerThread(void data)
{
    int i=((struct v)data)->i;
    int j=((struct v)data)->j;
    int accumulator = 0;

/*this is where you should calculate the assigned Cell. You will need to use the row(i) of
            A and column[j] of B. Accumulate the result in accumulator */
    int k;
    for(k=0; k<k; k++)
    {
            accumulator = A[i][k]*B[k][j];
    }
    C[i][j]=accumulator;
    pthread_exit(NULL);
}

 void printMatrix(int matrixIn, int rows, int columns)
{
    int matrix = matrixIn;
    int i,j;
    for (i=0;i<rows;i++)
    {
}

int main (int argc, char *argv[])
{

    pthread_t threads[M*N];
    int i,j;
    int counter = 0;
    int numThreadsCreated = 0;

    /*the following 5 lines demonstrates how to create 1 thread to calculate C[0][0], you
            will need to create a loop for all of C's cells*/
    struct v data = (struct v )malloc(sizeof(struct v));
    data->i = 0; //assign the row of C for thread to calculate
    data->j = 0; //assign the column of C for thread to calculate
    pthread_create(&threads[0], NULL, workerThread, data);
    numThreadsCreated++;

    /*wait for all the threads to finish before printing out the matrices*/
    for(j=0; j < numThreadsCreated; j++)
    {
            pthread_join( threads[j], NULL);
    }

    printf("A=\n");
    printMatrix(A,3,2);
    printf("B=\n");
    printMatrix(B,2,3);
    printf("A*B=\n");
    printMatrix(C,M,N);
    pthread_exit(NULL);
}
