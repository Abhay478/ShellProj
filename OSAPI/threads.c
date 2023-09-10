
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <fcntl.h> 
// #include <sys/shm.h> 
// #include <sys/stat.h>
// #include <sys/mman.h>
// #include <semaphore.h>
// #include <errno.h>
// #include <math.h>
// #include <pthread.h>

#include "osapi.h"

// Thread-spawner. No pthread_create()s outside this function.
void spawn(int count, T_t * offspring, void * (*thread_init)(void *), void * (*supply_func)(int) )
{
    T_t tid;
    A_t at;
    attrth(&at);
    for(int i = 0; i < count; i++){
        maketh(&tid, &at, thread_init, supply_func?supply_func(i):NULL);
        offspring[i] = tid;
    }
}

void join_all(int count, T_t * offspring)
{
    for(int i = 0; i < count; i++)
        jointh(offspring[i], NULL);
    
}
