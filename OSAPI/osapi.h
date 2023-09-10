#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <math.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
// #include "../Debug/debug.h"
#include <sys/stat.h>

#define maketh pthread_create
#define attrth pthread_attr_init
#define jointh pthread_join
#define exth pthread_exit
#define T_t pthread_t
#define A_t pthread_attr_t
#define gettid pthread_self
#define lock_t pthread_mutex_t
#define makelock pthread_mutex_init
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define unmakelock pthread_mutex_destroy

#define PARAMS PROT_READ | PROT_WRITE, MAP_SHARED
#define PARAMS_STD NULL, 1024, PARAMS, fd, 0
// macos doesn't have non-standard itoa. So, this.
#define itos(name, i) sprintf(name, "%d", i)

bool chkpt(int val) {
    if(val == -1){
        printf("%s\n", strerror(errno));
        return false;
    }
    else return true;
}


void spawn(int count, T_t * offspring, void * (*thread_init)(void *), void * (*supply_func)(int));
void join_all(int count, T_t * offspring);

void many_fork(int pid, int count, pid_t * offspring);
char * makeshm(char * name, int create);
void wait_all(int count, pid_t * offspring, void (*after_wait)(int));

int makesock();
void bindsock(int socket, int port, char * IP);
int othersock(int socket);
void pushsock(int socket, void * message, int len);
void * pullsock(int socket);
void connectsock(int socket, int port, char * IP);