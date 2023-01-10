#include "osh.h"
#include <math.h>
#ifdef itoa
#undef itoa
#endif

char * itoa(int n) {
    double l = log10(n);
    long int length = (l == (long int)l)?(long int)l:((long int)l + 1);
    if(n == 0 || n == 1) length = 1;

    char * out = calloc(length + 1, sizeof(char));
    for(int i = length - 1; i >= 0; i--) {
        int d = n % 10;
        n /= 10;
        out[i] = d + '0';
        if(n == 0) break;
    }

    return out;
}

void int_env(char * env, int n, int ov) {
    char * s = itoa(n);
    setenv(env, s, ov);
    free(s);
}

void redirect(char * file, int io) {
    while(isblank(*file)) file++;

    // char * fname = malloc()
    char * x = strchr(file, ' ');
    if(x) *x = 0;

    int fd;
    FILE * f = fopen(file, io?"w":"r");
    if(!f) {
        if(check_env("dbg")) printf("Unable to locate file: %s\n", file);
        return;
    }
    fd = fileno(f);
    // dup2(fd, io?STDIN_FILENO:STDOUT_FILENO);
    int_env("file", fd, 1);
    setenv("io", io?"1":"0", 1);

    if(check_env("dbg")) printf("fd: %d\n", fd);



}

int spl(char * temp, int * bg, char ** arg, char * cmd) {

        if(!strcmp(temp, "&")) {
            if(check_env("logs")) printf("& handled.\n");
            *bg = 1;
            return 1;
        }
        if(!strcmp(temp, "!!")) {
            if(check_env("logs")) printf("!! handled.\n");
            return 0;
        }
        if(!strcmp(temp, ">")) {
            redirect(cmd, STDOUT_FILENO);
            return 1;
        }
        if(!strcmp(temp, "<")) {
            redirect(cmd, STDIN_FILENO);
            return 1;
        }
        return -1;
}

int copy_arg(char ** arg, char ** temp) {
    *arg = malloc(ARG_LEN * sizeof(char));
    strlcpy(*arg, *temp, ARG_LEN); // this helps store the arguments in a vector
    if (!*arg) return 1;
    if(!**arg || isblank(**arg)) {
        if(check_env("logs")) printf("Empty arg.\n");
        if(check_env("mem")) free(*arg); 
        return 1;
    }
    if(check_env("dbg")) printf("%s\n", *arg);
    return 0;
}

// takes line and splits into argument vector.
char ** parse(char * cmd, int * bg) {
    // char ** out[2];
    char ** arg = malloc(N_ARGS * sizeof(char *));
    out[0] = arg;
    int i;
    for(i = 0; cmd && (i < N_ARGS); i++ ) {
        char * temp = strsep(&cmd, " "); // this shreds cmd, and temp points to a location within what used to be cmd.

        switch(spl(temp, bg, arg, cmd)) {
            case 0: return NULL;
            case 1: return arg;
            default: break;
        }

        if(!strcmp(temp, "~")) {
            temp = getenv("HOME");
        }
        if(temp[0] == '$') {
            temp = getenv(++temp);
        }
        
        if(copy_arg(arg + i, &temp)) break;
    }
    *bg = 0;

    if(check_env("logs")) printf("Args isolated.\n");

    return arg;
}


