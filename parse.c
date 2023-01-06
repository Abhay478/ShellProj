#include "osh.h"
extern int logs, dbg, mem, sarg;


// takes line and splits into argument vector.
char ** parse(char * cmd, int * bg) {
    char ** arg = malloc(N_ARGS * sizeof(char *));
    int i;
    for(i = 0; cmd && (i < N_ARGS); i++ ) {
        char * temp = strsep(&cmd, " "); // this shreds cmd, and temp points to a location within what used to be cmd.

        if(!strcmp(temp, "&")) {
            if(logs) printf("& handled.\n");
            *bg = 1;
            return arg;
        }
        if(!strcmp(temp, "!!")) {
            if(logs) printf("!! handled.\n");
            return NULL;
        }

        arg[i] = malloc(ARG_LEN * sizeof(char));
        strlcpy(arg[i], temp, ARG_LEN); // this helps store the arguments in a vector
        if (!arg[i]) break;
        if(!*arg[i] || isblank(*arg[i])) {
            if(logs) printf("Empty arg.\n");
            if(mem) free(arg[i]); 
            break;
        }

        if(dbg) printf("%s\n", arg[i]);
    }
    *bg = 0;

    if(logs) printf("Args isolated.\n");

    return arg;
}
