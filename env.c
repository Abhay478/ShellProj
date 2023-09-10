#include "osh.h"
// extern int dbg, logs, sarg, mem;
void show_env();
int env(char ** arg) { // basically a big if ladder
    if(check_env("logs")) {
        printf("Env commands.\n");
    }
    if(!strcmp(*arg, "show")) {
        show_env();
        return 0;
    }
    if(!strcmp(*arg, "dbg")) {
        if(check_env("dbg")) setenv("dbg", "0", 1);
        else setenv("dbg", "1", 1);

        return 0;
    }
    if(!strcmp(*arg, "logs")) {
        if(check_env("logs")) setenv("logs", "0", 1);
        else setenv("logs", "1", 1);

        return 0;
    }
    if(!strcmp(*arg, "sarg")) {
        if(check_env("sarg")) setenv("sarg", "0", 1);
        else setenv("sarg", "1", 1);

        return 0;
    }
    if(!strcmp(*arg, "mem")) {
        if(check_env("mem")) setenv("mem", "0", 1);
        else setenv("mem", "1", 1);

        return 0;
    }
    if(!strcmp(*arg, "bare")) {
        setenv("dbg", "0", 1);
        setenv("logs", "0", 1);
        setenv("sarg", "0", 1);
        setenv("mem", "0", 1);

        return 0;
    }
    if(!strcmp(*arg, "full")) {
        setenv("dbg", "1", 1);
        setenv("logs", "1", 1);
        setenv("sarg", "1", 1);
        // setenv("mem", "1", 1);

        return 0;
    }
    if(check_env("logs")) {
        printf("Env unchanged.\n");
    }
    return 1;

}

void v_env(char ** argv) { // iterative ecaller.
    for(int i = 0; argv[i]; i++) {
        if(env(&argv[i])) {
            printf("Invalid environment command: %s\n", argv[i]);
        }
    }
}

// prints status of Exec.
void what_exec(Exec * the) {
    static char s[] = "";
    if(!check_env("sarg")) return;
    if(!the->args) {printf("Clean.\n"); return;}
    printf("--------\n");

    if(check_env("logs")) printf("Args be: \n");
    for(int i = 0; the->args[i] && *(the->args[i]); i++)
        printf("%s%d: \"%s\"\n", s, i + 1, the->args[i]);

    if(check_env("logs")) printf("IO be: \n");
    printf("%sfd: %d -> %d\n", s, the->fd[0], the->fd[1]);

    if(the->pipee) {
        if(check_env("logs")) printf("Pipee be: \n");
        strcat(s, "\t");
        what_exec(the->pipee);
    }
    printf("--------\n");
}

// evar init.
void make_env() {
    setenv("dbg", "0", 0);
    setenv("logs", "0", 0);
    setenv("sarg", "0", 0);
    setenv("mem", "0", 0);
    setenv("caret", "0", 0);
    setenv("PATH", "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/TeX/texbin:/Users/abhay/.cabal/bin:/Users/abhay/.ghcup/bin:/Library/Frameworks/Python.framework/Versions/3.10/bin:/Users/abhay/.cargo/bin", 0);
    setenv("HOME", "/Users/abhay", 0);

}

// evar display
void show_env() {
    printf("dbg = %s\n", getenv("dbg"));
    printf("logs = %s\n", getenv("logs"));
    printf("sarg = %s\n", getenv("sarg"));
    printf("mem = %s\n", getenv("mem"));
}
