#include "osh.h"

// nomenclature neurolexical

void strip_descriptors(Exec * done) {
    Exec * current = done;
    while(current) {
        if(current->fd[0] != 0) close(current->fd[0]);
        if(current->fd[1] != 1) close(current->fd[1]);
        current = current->pipee;
    }
}

void push(History * the, Exec * done) {
    if(the->hist[the->capac]) {
        if(check_env("mem")) free(the->hist[the->capac]);
    }
    the->hist[the->capac] = done;
    the->capac = (1 + the->capac) % MEMORY;
    
}

History * init() {
    History * out = malloc(sizeof(History));
    out->capac = 0;
    out->hist = calloc(MEMORY, sizeof(void *));
    return out;
}

Exec * pop(History * the) {
    if(the->capac < 0) return NULL;
    the->capac = (the->capac - 1) % MEMORY;
    Exec * out = the->hist[the->capac];
    return out;
}

Exec * peep(History * the, int age) {
    if(the->capac < 0) return NULL;
    Exec * out = the->hist[(the->capac - age) % MEMORY];
    return out;
}

void lore(History * the) {
    char * full = "full";
    env(&full);
    for(int i = 0; i < MEMORY; i++) {
        if(the->hist[i]) {
            what_exec(the->hist[i]);
            printf("********\n");
        }
    }
    char * bare = "bare";
    env(&bare);
}