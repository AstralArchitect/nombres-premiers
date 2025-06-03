#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <stdexcept>

void checkCudaError(cudaError_t err, const char* msg) {
    if (err != cudaSuccess) {
        std::cerr << "CUDA Error: " << msg << ": " << cudaGetErrorString(err) << std::endl;
        // In a real application, you might throw an exception or exit.
        exit(EXIT_FAILURE);
    }
}

__device__ unsigned int nbPrimesFound = 1;

__device__ bool estPremier(unsigned int const& n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;

    if (n % 2 == 0) return false;
    if (n % 3 == 0) return false;

    for (unsigned int i = 5; i * i < n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

__global__ void find_kernel(unsigned int fin, bool *isPrimes)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;;
    unsigned int nbPrimesFound_local = 0;

    for (unsigned int i = idx; i < fin / 2; i += gridDim.x * blockDim.x){
        if (estPremier(i * 2 + 3))
        {
            isPrimes[i] = true;
            nbPrimesFound_local++;
        }
    }

    atomicAdd(&nbPrimesFound, nbPrimesFound_local);
}

unsigned int *find_to_n(unsigned int const& fin, unsigned int &numPrimesFound) {
    // Handle cases where 0 primes are requested.
    if (fin < 2) return NULL;

    const unsigned int ARRAY_SIZE = (fin / 2) - 1;

    // initialisation du tableau
    bool *isPrimes = new bool[ARRAY_SIZE];
    memset(isPrimes, 0, (ARRAY_SIZE) * sizeof(bool));
    
    bool * isPrimes_d;
    // Allocate memory on the device for the boolean array
    checkCudaError(cudaMalloc(&isPrimes_d, ARRAY_SIZE * sizeof(bool)), "cudaMalloc isPrimes_d failed");
    // Copy initial (all false) data from host to device
    checkCudaError(cudaMemcpy(isPrimes_d, isPrimes, ARRAY_SIZE * sizeof(bool), cudaMemcpyHostToDevice), "cudaMemcpy isPrimes H2D failed");

    // Get device properties for kernel launch configuration
    int deviceId;
    checkCudaError(cudaGetDevice(&deviceId), "cudaGetDevice failed"); // Get the current device ID

    cudaDeviceProp deviceProp;
    checkCudaError(cudaGetDeviceProperties(&deviceProp, deviceId), "cudaGetDeviceProperties failed");

    // 1. Choose threads per block.
    int threadsPerBlock = 1024;
    if (threadsPerBlock > deviceProp.maxThreadsPerBlock) {
        threadsPerBlock = deviceProp.maxThreadsPerBlock;
        std::cerr << "Warning: threadsPerBlock adjusted to device max: " << threadsPerBlock << std::endl;
    }

    // 2. Calculate blocks per grid.
    int blocksPerGrid = (ARRAY_SIZE + threadsPerBlock - 1) / threadsPerBlock;

    // Optional: Check if blocksPerGrid exceeds the maxGridSize for the x-dimension
    if (blocksPerGrid > deviceProp.maxGridSize[0]) {
        std::cerr << "Warning: Calculated blocksPerGrid (" << blocksPerGrid
                  << ") exceeds device's maxGridSize[0] (" << deviceProp.maxGridSize[0]
                  << "). Capping grid size." << std::endl;
        blocksPerGrid = deviceProp.maxGridSize[0]; // Cap it if it exceeds
    }

    // Launch the kernel!
    find_kernel<<<blocksPerGrid, threadsPerBlock>>>(fin, isPrimes_d);
    checkCudaError(cudaGetLastError(), "find_kernel launch failed"); // Check for errors immediately after launch

    // Synchronize to ensure the kernel finishes execution
    checkCudaError(cudaDeviceSynchronize(), "cudaDeviceSynchronize failed");

    // Copy results back from device to host
    checkCudaError(cudaMemcpy(isPrimes, isPrimes_d, ARRAY_SIZE * sizeof(bool), cudaMemcpyDeviceToHost), "cudaMemcpy D2H failed");
    
    // Copy the final prime count from device global memory to host
    checkCudaError(cudaMemcpyFromSymbol(&numPrimesFound, nbPrimesFound, sizeof(unsigned int)), "cudaMemcpyFromSymbol nbPrimesFound failed");

    // transform to int array
    unsigned int *primes = new unsigned int[fin];
    primes[0] = 2;
    numPrimesFound = 1;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (!isPrimes[i]) continue;
        primes[numPrimesFound] = i * 2 + 3;
        numPrimesFound++;
    }

    delete [] isPrimes;
    cudaFree(isPrimes_d);
    return primes;
}

inline bool estPremier(unsigned int const& n, unsigned int *primes) {
    for (unsigned int i = 2; primes[i] * primes[i] <= n; i++)
        if (n % primes[i] == 0)
            return false;
    
    return true;
}

unsigned int *find_n_primes(unsigned int const& fin) {
    // Handle cases where 0 primes are requested.
    if (fin == 0) return NULL;

    // variables
    unsigned int *primes;
    unsigned int numPrimesFound = 2;

    // allocation dynamique de mémoire
    if ((primes = (unsigned int*)malloc((fin + 1) * sizeof(unsigned int))) == NULL)
        return NULL;
    primes[0] = 2;
    if (fin == 1) return primes;

    primes[1] = 3;
    if (fin == 2) return primes;

    primes[2] = 5;
    unsigned int nTest = 5;
    while (numPrimesFound < fin){
        if (estPremier(nTest, primes))
        {
            primes[numPrimesFound] = nTest;
            numPrimesFound++;
        }
        if (estPremier(nTest + 2, primes))
        {
            primes[numPrimesFound] = nTest + 2;
            numPrimesFound++;
        }
        
        nTest += 6;
    }
    
    return primes;
}