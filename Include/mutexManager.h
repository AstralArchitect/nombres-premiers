#include <pthread.h>

typedef struct
{
    unsigned long var;
    pthread_mutex_t mutex;
} MutexedVar;

void initVar(MutexedVar *var, unsigned long value);
void increase(MutexedVar *var, unsigned long value);
void set(MutexedVar *var, unsigned long value);