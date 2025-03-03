#include <iostream>
#include <cuda_runtime.h>

#include <algorithm>
#include <execution>

__device__ unsigned int numPrimesFound = 1;

__device__ inline bool estPremier(unsigned int const& n) {
    if (n % 2 == 0 || n % 3 == 0)
        return false;

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
    int stride = blockDim.x;

    if (id == 0) {
        primes[0] = 2;
    }
    __syncthreads();

    unsigned int n = 3 + id;
    printf("thread id :%d\n", id);
    printf("n = %d\n", n);
    while (true)
    {
        if (estPremier(n))
        {
            unsigned int index = atomicAdd((unsigned int*)&numPrimesFound, 1);
            if (index < fin)
                primes[index] = n;
            else
                break;
        }
        n += stride;

        // Synchronize threads and check if the desired number of primes is found
        __syncthreads();
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
        fprintf(stderr, "\nFailed to allocate device memory (error code %s)!\n", cudaGetErrorString(err));
        return nullptr;
    }

    // Get the max number of blocks and threads
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    int blockSize = prop.maxThreadsPerBlock;
    int numBlocks = (fin + blockSize - 1) / blockSize; // Calculate the number of blocks needed

    // Call the kernel
    search_kernel<<<1, 1>>>(d_primes, fin);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "\nFailed to launch kernel (error code %s)!\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        return nullptr;
    }

    // Synchronize the device
    err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        fprintf(stderr, "\nFailed to synchronize device (error code %s)!\n", cudaGetErrorString(err));
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

    std::sort(primes, primes + fin);

    // Free the memory on the device
    cudaFree(d_primes);

    return primes;
}
