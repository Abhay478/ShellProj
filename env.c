#include "osh.h"
extern int dbg, logs, sarg, mem;

int env(char * arg) {
    if(!strcmp(arg, "exit")) return -1;
    if(!strcmp(arg, "dbg")) {
        if(dbg) dbg = 0;
        else dbg = 1;

        return 1;
    }
    if(!strcmp(arg, "logs")) {
        if(logs) logs = 0;
        else logs = 1;

        return 1;
    }
    if(!strcmp(arg, "sarg")) {
        if(sarg) sarg = 0;
        else sarg = 1;

        return 1;
    }
    if(!strcmp(arg, "mem")) {
        if(mem) mem = 0;
        else mem = 1;

        return 1;
    }
    if(!strcmp(arg, "bare")) {
        dbg = 0;
        logs = 0;
        sarg = 0;
        mem = 0;
    }
    if(!strcmp(arg, "full")) {
        dbg = 1;
        logs = 1;
        sarg = 1;
        mem = 1;
    }

    return 0;

}

void v_env(char ** argv) {
    for(int i = 0; argv[i]; i++) {
        if(!env(argv[i])) {
            printf("Invalid envvironment setting: %s\n", argv[i]);
        }
    }
}


void what_args(char ** arg) {
    if(!arg) {printf("Clean.\n"); return;}
    for(int i = 0; arg[i] && *arg[i]; i++)
        printf("%d: \"%s\"\n", i + 1, arg[i]);
}
