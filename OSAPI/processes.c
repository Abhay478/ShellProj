
#include "osapi.h"


// Recursive process-spawner. No forks outside this function.
void many_fork(int pid, int count, pid_t * offspring)
{
    if(pid){
        offspring[count] = pid; 
        if(count) many_fork(fork(), count - 1, offspring);
    } // parent
    else return;
    
}

// Creates a shared memory object with convenient parameters. No shared memory creation outside this function.
char * makeshm(char * name, int create)
{
    int fd;
    if(create) fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    else fd = shm_open(name, O_RDWR, 0666);

    if(!chkpt(fd))
        return (char *) -1;
    

    ftruncate(fd, 1024);

    char * out = mmap(PARAMS_STD);

    return out;

}

void wait_all(int count, pid_t * offspring, void (*after_wait)(int))
{
    for(int i = 0; i < count; i++){
        if(chkpt(waitpid(offspring[i], NULL, WUNTRACED)))
            after_wait(offspring[i]);
    }
}

