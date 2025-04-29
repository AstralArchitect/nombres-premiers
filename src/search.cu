#include <iostream>
#include <cuda_runtime.h>

#include <algorithm>
#include <execution>

__device__ unsigned int numPrimesFound = 2;

__device__ inline bool estPremier(unsigned int const& n) {
    if (n % 3 == 0) return false;

    // Check for divisibility by numbers of the form 6k ± 1 up to sqrt(n)
    
    for (unsigned int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0)
            return false;

        if (n % (i + 2) == 0)
            return false;
    }
    return true;
}

__global__ void search_kernel(unsigned int *primes, unsigned int fin)
{
    int id = threadIdx.x;
    int stride = blockDim.x * 2;

    unsigned int n = 5 + id * 2;
    while (numPrimesFound < fin)
    {
        if (estPremier(n))
        {
            unsigned int index = atomicAdd((unsigned int*)&numPrimesFound, 1);
            primes[index] = n;
        }
        n += stride;
        __syncthreads();
    }
}

unsigned int* find(unsigned int const& fin)
{
    // Allocate memory on the device for primes
    unsigned int *d_primes;
    cudaError_t err = cudaMalloc(&d_primes, fin * sizeof(unsigned int));
    if (err != cudaSuccess) {
        fprintf(stderr, "\nFailed to allocate device memory (error : %s)!\n", cudaGetErrorString(err));
        return nullptr;
    }

    unsigned int initials[3] = { 2, 3, 5 };

    // Copy initial primes to the device
    err = cudaMemcpy(d_primes, &initials[0], sizeof(initials), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        fprintf(stderr, "\nFailed to copy initial primes to the device memory (error : %s)!\n", cudaGetErrorString(err));
        return nullptr;
    }

    // Get the max number of blocks and threads
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    int blockSize = prop.maxThreadsPerBlock;
    int numBlocks = (fin + blockSize) / blockSize; // Calculate the number of blocks needed

    if (blockSize > fin)
        blockSize = fin;

    // Call the kernel
    search_kernel<<<numBlocks, blockSize>>>(d_primes, fin);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "\nFailed to launch kernel (error : %s)!\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        return nullptr;
    }

    // Synchronize the device
    err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        fprintf(stderr, "\nFailed to synchronize device (error : %s)!\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        return nullptr;
    }

    // Copy the result back to the host
    unsigned int *primes = new unsigned int[fin];
    err = cudaMemcpy(primes, d_primes, fin * sizeof(unsigned int), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy memory from device to host (error : %s)!\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        delete[] primes;
        return nullptr;
    }

    std::sort(primes, primes + fin);

    // Free the memory on the device
    cudaFree(d_primes);

    return primes;
}
