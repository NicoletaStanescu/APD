#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <cstdlib>
#include <ctime>

#define NO_ELEMENTS 8

__global__ void median_filter(int *input, int *output)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	int start_col = 0;

	int start_row = 0;
	int sum = 0;
	int end_row = NO_ELEMENTS - 1;
	int end_col = NO_ELEMENTS - 1;

	if(row > 0)
	{
		start_row = row - 1;
	}
	if(col > 0)
	{
		start_col = col - 1;
	}
	if(row < NO_ELEMENTS - 1)
	{
		end_row = row + 1;
	}
	if(col < NO_ELEMENTS - 1)
	{
		end_col = col + 1;
	}

	int number_of_elements = 0;


    //parcurgerea matricei de pixeli
	for(auto indexRow = start_row; indexRow <= end_row; ++indexRow)
	{
		for (auto indexCol = start_col; indexCol <= end_col; ++indexCol)
		{
			// make sure we don't add the elemnt itself in the sum
			if (indexCol != col || indexRow != row)
			{
				sum += input[indexRow * NO_ELEMENTS + indexCol];
				number_of_elements++;
			}
		}
	}

	output[row * NO_ELEMENTS + col] = sum / number_of_elements;
}

void random_ints(int *array);
void print_array(int *array);

int main()
{
	int *input;
	int *output;
	int *dev_input, *dev_output;

	int size = NO_ELEMENTS * NO_ELEMENTS * sizeof(int);

	img_location = "C:\Users\cuda_2_Img";

	// Aloca spațiu pentru copiile dispozitivului
	cudaMalloc((void **)&dev_input, size);
	cudaMalloc((void **)&dev_output, size);

	// configuram variabilele
	input = new int[size];
	output = new int[size];

	srand(time(nullptr));

	random_ints(input);

	printf("\Input:\n");
	print_array(input);

	// copiem intrarile in dispozitiv
	cudaMemcpy(dev_input, input, size, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_output, output, size, cudaMemcpyHostToDevice);

	int numOfBlocks = 1;
	dim3 threadsPerBlock(NO_ELEMENTS, NO_ELEMENTS);

	median_filter <<< numOfBlocks, threadsPerBlock >>> (dev_input, dev_output);

	cudaMemcpy(output, dev_output, size, cudaMemcpyDeviceToHost);

	printf("\Output: \n");
	print_array(output);

	// elibereaza memoria dispozitivului
	cudaFree(dev_input);
	cudaFree(dev_output);

	// elibereaza memoria host
	free(input);
	free(output);

	return 0;
}
//sortarea pixelilor
void random_ints(int *array)
{
	for (auto i = 0; i < NO_ELEMENTS; ++i)
	{
		for (auto j = 0; j < NO_ELEMENTS; ++j)
		{
			array[i * NO_ELEMENTS + j] = rand() % 10;
		}
	}
}
//afisarea pixelilor
void print_array(int *array)
{
	for (auto i = 0; i < NO_ELEMENTS; ++i)
	{
		for (auto j = 0; j < NO_ELEMENTS; ++j)
		{
			printf("%d ", array[i * NO_ELEMENTS + j]);
		}

		printf("\n");
	}
}
