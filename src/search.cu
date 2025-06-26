#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <stdexcept>

typedef unsigned int uint;

void checkCudaError(cudaError_t err, const char* msg) {
    if (err != cudaSuccess) {
        std::cerr << "CUDA Error: " << msg << ": " << cudaGetErrorString(err) << std::endl;
        // In a real application, you might throw an exception or exit.
        exit(EXIT_FAILURE);
    }
}

__global__ void find_kernel(uint fin, bool *isPrimes)
{
    // Each block tests a different number 'i' in a grid-striding loop.
    for (uint i = 3 + blockIdx.x; i <= fin; i += gridDim.x)
    {
        // 1. Correctly declare and initialize a shared flag for the block.
        //    Only one thread needs to do the initialization.
        __shared__ bool isPrime_flag;
        if (threadIdx.x == 0) {
            isPrime_flag = true;
        }
        // Synchronize to make sure isPrime_flag is visible to all threads.
        //__syncthreads();

        // 2. Perform parallel trial division.
        //    Let all threads check their assigned divisors.
        //    The loop condition is now much simpler and safer.
        //    The check is j*j <= i, not i/2
        for (uint j = 3 + threadIdx.x; j * j <= i; j += blockDim.x)
        {
            // If the flag is already false, we can skip the check. This is a safe optimization.
            if (!isPrime_flag) {
                break;
            }

            if (i % j == 0) {
                // A divisor is found. Set the shared flag to false.
                // This is a benign race condition, as all threads write the same value (false).
                isPrime_flag = false;
            }
        }

        // 3. Synchronize to ensure the final result of isPrime_flag is visible to all threads.
        //__syncthreads();

        // 4. Have ONLY ONE thread write the result to global memory to avoid a race condition.
        if (threadIdx.x == 0 && isPrime_flag == true)
        {
            // Assuming isPrimes maps odd numbers. If i=3, idx=0. If i=5, idx=1. So (i-3)/2
            isPrimes[(i - 3) / 2] = true;
        }
    }
}

uint *find_to_n(uint const& fin, uint &numPrimesFound) {
    // Handle cases where 0 primes are requested.
    if (fin < 2) return NULL;

    const uint ARRAY_SIZE = (fin / 2) - 1;

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
    int threadsPerBlock = deviceProp.maxThreadsPerBlock;

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
    
    find_kernel<<<blocksPerGrid, threadsPerBlock, sizeof(bool)>>>(fin, isPrimes_d);
    checkCudaError(cudaGetLastError(), "find_kernel launch failed"); // Check for errors immediately after launch

    // Synchronize to ensure the kernel finishes execution
    checkCudaError(cudaDeviceSynchronize(), "cudaDeviceSynchronize failed");

    // Copy results back from device to host
    checkCudaError(cudaMemcpy(isPrimes, isPrimes_d, ARRAY_SIZE * sizeof(bool), cudaMemcpyDeviceToHost), "cudaMemcpy D2H failed");

    // transform to int array
    uint *primes = new uint[fin];
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

inline bool estPremier(uint const& n, uint *primes) {
    for (uint i = 2; primes[i] * primes[i] <= n; i++)
        if (n % primes[i] == 0)
            return false;
    
    return true;
}

uint *find_n_primes(uint const& fin) {
    // Handle cases where 0 primes are requested.
    if (fin == 0) return NULL;

    // variables
    uint *primes;
    uint numPrimesFound = 2;

    // allocation dynamique de mémoire
    if ((primes = (uint*)malloc((fin + 1) * sizeof(uint))) == NULL)
        return NULL;
    primes[0] = 2;
    if (fin == 1) return primes;

    primes[1] = 3;
    if (fin == 2) return primes;

    primes[2] = 5;
    uint nTest = 5;
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