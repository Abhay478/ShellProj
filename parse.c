#include "osh.h"
#include <math.h>
#ifdef itoa
#undef itoa
#endif

extern History * his;

// Mac has no itoa, so this.
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

// so that I can set the value of an environment variable to the value of a local variable
void int_env(char * env, int n, int ov) {
    char * s = itoa(n);
    setenv(env, s, ov);
    free(s);
}

// I reinvented some syntax, [file] for input, {file} for output.
void redirect(Exec * the, char * fp, char * lp) {
    
    char * G, * L, *infile, *outfile;
    char * g = strrchr(the->cmd, '{'); // only the last one.
    char * l = strchr(the->cmd, '['); // only the first one.
    if(g) {
        if(fp || lp) return;
        // if(lp && g < lp) return;
        // because any output redirection before piping is evil.

        // grunt work
        G = strrchr(g, '}');
        outfile = calloc(G - g, sizeof(char));
        strncpy(outfile, g + 1, G - g - 1);
        FILE * f = fopen(outfile, "w");

        the->fd[1] = fileno(f); // this is why using the struct was nice.
        if(check_env("dbg")) printf("fd: %d %d\n", the->fd[0], the->fd[1]);
        free(outfile);
    }
    if(l) {
        if(fp && l > fp) return;
        // because any input direction after piping is evil.

        // grunt work 2.0
        L = strstr(l, "]");
        infile = calloc(L - l, sizeof(char));
        strncpy(infile, l + 1, L - l - 1);
        FILE * f = fopen(infile, "r");
        if(!f) {
            if(check_env("dbg")) printf("Unable to locate file: %s\n", infile);
            free(infile);
            return;
        }

        the->fd[0] = fileno(f); // yay structs
        if(check_env("dbg")) printf("fd: %d %d\n", the->fd[0], the->fd[1]);
        free(infile);
    }

    if(!g && !l) {
        if(check_env("logs")) printf("No redirection.\n"); // thank god.
    }

}

int spl(char * temp, Exec * the) {
    if(!strcmp(temp, "&")) {
        if(check_env("logs")) printf("& handled.\n");
        the->fgbg = 1;
        return -7; // guess why?
    }
    if(!strcmp(temp, "!!")) {
        if(check_env("logs")) printf("!! handled.\n");
        return -11; // .
    }
    if(temp[0] == '^') {
        if(check_env("logs")) printf("^ handled.\n");
        setenv("caret", "1", 1);
        return atoi(temp + 1); // offset, basically.
    }
    if(!strcmp(temp, "|")) { // .
        if(check_env("logs")) printf("| handled.\n");
        char * tb = temp + 2; // Spacing restrictions - at least one space between | and other stuff.
        the->pipee = build_exec(&tb, 1); 
        // the recursive stuff that potentially makes multiple pipes work - 
        // remember, we called build_exec() in main, around four stack levels ago.
        return 0;

    }
    return -1;
}

// strlcpy wrapper.
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

// initialise Exec object.
Exec * setup(char * cmd, int bg) {
    Exec * out = calloc(1, sizeof(Exec));
    out->args = malloc(N_ARGS * sizeof(char *));
    out->cmd = malloc(strlen(cmd) * sizeof(char));

    strlcpy(out->cmd, cmd, ARG_LEN);
    out->fgbg = bg;
    out->fd[0] = 0;
    out->fd[1] = 1;
    out->pipee = NULL;
    char * fp = strchr(out->cmd, '|'); // first pipe
    char * lp = strrchr(out->cmd, '|'); // last pipe

    redirect(out, fp, lp); // here's hoping this works, eh?
    return out;
}

// takes line and splits into argument vector.
Exec * parse(char * cmd, int bg) {
    Exec * out = setup(cmd, bg); // skeleton

    int i;
    for(i = 0; cmd && (i < N_ARGS); i++) {
        char * temp = strsep(&cmd, " "); // this shreds cmd, and temp points to a location within what used to be cmd.
        if(temp[0] == '{' || temp[0] == '[') continue; // skips redirection, we already handled that.
        int sp = spl(temp, out);
        switch(sp) {
            case -11: return pop(his); // bangbang
            case -7: return out; // & has to be at the end
            case -1 : break; // nothing, keep going.
            case 0: return out; // Dem pipes, man.
            default : return peep(his, sp); // caret
        }
        setenv("caret", "0", 1); // Coz we've an actual command on our hands.

        // grunt work - ugh.
        if(!strcmp(temp, "~")) {
            temp = getenv("HOME");
        }
        if(temp[0] == '$') {
            temp = getenv(++temp); // add more of these
        }
        
        // takes string and puts in struct field.
        if(copy_arg(out->args + i, &temp)) break;
    }

    if(check_env("logs")) printf("Args isolated.\n");

    return out;
}

