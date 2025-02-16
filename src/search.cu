#include <iostream>
#include <cuda_runtime.h>

__device__ volatile unsigned int numPrimesFound = 1;

__device__ inline bool estPremier(unsigned int const& n) {
    for (unsigned int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    
    return true;
}

__global__ void search_kernel(unsigned int *primes, unsigned int fin)
{
    int id = threadIdx.x;
    int stride = blockDim.x;

    if (id == 0) {
        primes[0] = 2;
    }
    __syncthreads();

    int n = 3 + id;
    while (true)
    {
        if (estPremier(n))
        {
            unsigned int index = atomicAdd((unsigned int*)&numPrimesFound, 1);
            primes[index] = n;
        }
        n += stride;

        // Synchronize threads and check if the desired number of primes is found
        if (numPrimesFound >= fin) {
            break;
        }
    }
}

unsigned int* find_gpu(unsigned int const& fin)
{
    // Allocate memory on the device for primes
    unsigned int *d_primes;
    cudaError_t err = cudaMalloc(&d_primes, fin * sizeof(unsigned int));
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate device memory (error code %s)!\n", cudaGetErrorString(err));
        return nullptr;
    }
    
    // Get the max number of blocks and threads
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    int blockSize = prop.maxThreadsPerBlock;
    int numBlocks = (fin + blockSize - 1) / blockSize; // Calculate the number of blocks needed
    
    // Call the kernel
    search_kernel<<<numBlocks, blockSize>>>(d_primes, fin);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to launch kernel (error code %s)!\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        return nullptr;
    }
    
    // Synchronize the device
    err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to synchronize device (error code %s)!\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        return nullptr;
    }
    
    // Copy the result back to the host
    unsigned int *primes = new unsigned int[fin];
    err = cudaMemcpy(primes, d_primes, fin * sizeof(unsigned int), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy memory from device to host (error code %s)!\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        delete[] primes;
        return nullptr;
    }
    
    // Free the memory on the device
    cudaFree(d_primes);
    
    return primes;
}