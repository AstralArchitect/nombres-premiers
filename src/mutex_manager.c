#include <mutexManager.h>

void initVar(MutexedVar *var, unsigned long value)
{
    var->var = value;
    pthread_mutex_t newMutex = PTHREAD_MUTEX_INITIALIZER;
    var->mutex = newMutex;
}

void increase(MutexedVar *var, unsigned long value)
{
    pthread_mutex_lock(&var->mutex);
    var->var += value;
    pthread_mutex_unlock(&var->mutex);
}

void set(MutexedVar *var, unsigned long value)
{
    pthread_mutex_lock(&var->mutex);
    var->var = value;
    pthread_mutex_unlock(&var->mutex);
}