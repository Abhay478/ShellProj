#include "osh.h"
#include <math.h>
#ifdef itoa
#undef itoa
#endif

extern History * his;

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

void redirect(Exec * the) {
    char * p = strstr(the->cmd, "|");
    if(p) return;
    char * G, * L, *infile, *outfile;
    char * g = strstr(the->cmd, "{");
    char * l = strstr(the->cmd, "[");
    if(g) {
        G = strstr(g, "}");
        outfile = calloc(G - g, sizeof(char));
        strncpy(outfile, g + 1, G - g - 1);
        FILE * f = fopen(outfile, "w");
        the->fd[1] = fileno(f);
        if(check_env("dbg")) printf("fd: %d %d\n", the->fd[0], the->fd[1]);
    }
    if(l) {
        L = strstr(l, "]");
        infile = calloc(L - l, sizeof(char));
        strncpy(infile, l + 1, L - l - 1);
        FILE * f = fopen(infile, "r");
        if(!f) {
            if(check_env("dbg")) printf("Unable to locate file: %s\n", infile);
            return;
        }
        the->fd[0] = fileno(f);
        if(check_env("dbg")) printf("fd: %d %d\n", the->fd[0], the->fd[1]);
    }

    if(!g && !l) {
        if(check_env("logs")) printf("No redirection.\n");
    }

}

int spl(char * temp, Exec * the) {

    if(!strcmp(temp, "&")) {
        if(check_env("logs")) printf("& handled.\n");
        the->fgbg = 1;
        return -7;
    }
    if(!strcmp(temp, "!!")) {
        if(check_env("logs")) printf("!! handled.\n");
        return -11;
    }
    if(temp[0] == '^') {
        if(check_env("logs")) printf("^ handled.\n");
        setenv("caret", "1", 1);
        return atoi(temp + 1);
    }
    if(!strcmp(temp, "|")) {
        if(check_env("logs")) printf("| handled.\n");
        // the->pipee = setup(temp + 2, 0);
        char * tb = temp + 2;
        the->pipee = build_exec(&tb, 1);
        return 0;

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

Exec * setup(char * cmd, int bg) {
    Exec * out = calloc(1, sizeof(Exec));
    out->args = malloc(N_ARGS * sizeof(char *));
    out->cmd = malloc(strlen(cmd) * sizeof(char));

    strlcpy(out->cmd, cmd, ARG_LEN);
    out->fgbg = bg;
    out->fd[0] = 0;
    out->fd[1] = 1;
    out->pipee = NULL;

    redirect(out);
    // char * to0 = strstr(cmd, " > ");
    // if(to0) *to0 = 0;
    // to0 = strstr(cmd, " < ");
    // if(to0) *to0 = 0;
    return out;
}

// takes line and splits into argument vector.
Exec * parse(char * cmd, int bg) {
    Exec * out = setup(cmd, bg);

    int i;
    for(i = 0; cmd && (i < N_ARGS); i++ ) {
        char * temp = strsep(&cmd, " "); // this shreds cmd, and temp points to a location within what used to be cmd.
        if(temp[0] == '{' || temp[0] == '[') continue; // skips redirection.
        int sp = spl(temp, out);
        switch(sp) {
            case -11: return pop(his); // bangbang
            case -7: return out; // & has to be at the end
            case -1 : break;
            case 0: return out;
            default : return peep(his, sp);
        }
        setenv("caret", "0", 1);

        if(!strcmp(temp, "~")) {
            temp = getenv("HOME");
        }
        if(temp[0] == '$') {
            temp = getenv(++temp); // add more of these
        }
        
        if(copy_arg(out->args + i, &temp)) break;
    }

    if(check_env("logs")) printf("Args isolated.\n");

    return out;
}

