#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <cuda.h>
#include <cuda_runtime.h>

__device__ bool estPremier(unsigned long n)
{
    for (unsigned long j = 2; j * j <= n; j++) {
        if (n % j == 0){
            return false;
            break;
        }
    }
    return true;
}

__global__ void thread(unsigned long fin, unsigned long *nombresPremiersTrouves, unsigned long *liste){
    int tid = blockIdx.x * blockDim.x + threadIdx.x + 2;

    while (true) {
        if (estPremier(tid)) {
            int idx = atomicAdd((int*)nombresPremiersTrouves, 1);
            if (idx < fin) {
                liste[idx] = tid;
            }
        }
        tid += gridDim.x * blockDim.x;

        if (*nombresPremiersTrouves >= fin) {
            break;
        }
    }
}

unsigned long *find(unsigned long fin) {
    // host variables
    unsigned long *liste, nombresPremiers = 0;
    //device variables
    unsigned long *liste_d, *nombresPremiers_d;

    // allocation dynamique de mémoire sur le GPU
    cudaMalloc(&liste_d, fin * sizeof(unsigned long));
    if (liste_d == NULL)
    {
        return NULL;
    }
    cudaMalloc(&nombresPremiers_d, sizeof(unsigned long));
    if (nombresPremiers_d == NULL)
    {
        cudaFree(liste_d);
        return NULL;
    }

    //copie de mémoire sur le GPU
    cudaMemcpy(nombresPremiers_d, &nombresPremiers, sizeof(unsigned long), cudaMemcpyHostToDevice);

    //apele des threads GPU
    int block_size = 512;
    int grid_size = ((fin + block_size) / block_size);
    thread<<<grid_size,block_size>>>(fin, nombresPremiers_d, liste_d);

    // allocation dynamique de mémoire sur le host
    liste = (unsigned long*)malloc(fin * sizeof(unsigned long));

    if (liste == NULL)
    {
        cudaFree(nombresPremiers_d);
        cudaFree(liste_d);
        return NULL;
    }

    // copie du resultat de la recherche sur le host
	cudaMemcpy(liste, liste_d, fin * sizeof(unsigned long), cudaMemcpyDeviceToHost);

    // liberation de la memoire allouée sur le GPU
    cudaFree(nombresPremiers_d);
    cudaFree(liste_d);

    return liste;
}