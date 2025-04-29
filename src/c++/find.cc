#include <iostream>
#include <cstdlib>
#include <chrono>

extern "C" void search_square(unsigned int*, unsigned int);
extern "C" void search_square_root(unsigned int*, unsigned int);

unsigned int *find(unsigned int const& fin)
{
    unsigned int *primes = (unsigned int*)malloc(fin * sizeof(unsigned int));
    if (!primes) {
        std::cerr << "Error: malloc failed" << std::endl;
        exit(1);
    }

    search_square(primes, fin);

    return primes;
}