#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <conio.h>

using namespace std;


__device__
int Min(int a, int b) { return a < b ? a : b; }

__global__ void RoyFloyd(int* mat, int k, int N) {

  int i = threadIdx.x + blockIdx.x * blockDim.x;
  int j = threadIdx.y + blockIdx.y * blockDim.y;

  if (i < N && j < N) {
    if (mat[i*N + k] != -1 && mat[k*N + j] != -1) {
      if (mat[i*N+j] == -1) {
        mat[i*N+j] = mat[i*N + k] + mat[k*N +j];
      } else {
        mat[i*N+j] = Min(mat[i*N + k] + mat[k*N + j], mat[i*N+j]);
      }
    }
  }

}


int main(int argc, char **argv) {

  int thread_per_block = 512, i, j;
  int* cuda_mat;
  size_t N;

  ifstream f("date.in");
  f >> N;

  cout <<" n = "  << N<<endl;


  int size = sizeof(int)* N * N;

  int *mat = (int*)malloc(sizeof(int)*N*N);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int v;
      f >> v;
      mat[i * N + j] = v;
    }
  }

  int *result = (int*)malloc(sizeof(int)*N*N);

  clock_t t;
  t = clock();

  cudaMalloc((void**)&cuda_mat, size);
  cudaMemcpy(cuda_mat, mat, size, cudaMemcpyHostToDevice);
  int num_block = ceil((float)(N*N / (thread_per_block)));

  for (int k = 0; k < N; ++k) {
    RoyFloyd << <num_block, (thread_per_block) >> >(cuda_mat, k, N);
  }
  cudaMemcpy(result, cuda_mat, size, cudaMemcpyDeviceToHost);
  cudaFree(cuda_mat);
  t = clock() - t;

  cout<<" execution time: "<<((double)t)/CLOCKS_PER_SEC<<endl;

  getchar();
}
