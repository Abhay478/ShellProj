#include "osh.h"
// extern int dbg, logs, sarg, mem;
void show_env();
int env(char ** arg) {
    if(!strcmp(*arg, "exit")) exit(0);
    if(!strcmp(*arg, "show")) {
        show_env();
        return 1;
    }
    if(!strcmp(*arg, "dbg")) {
        if(check_env("dbg")) setenv("dbg", "0", 1);
        else setenv("dbg", "1", 1);

        return 1;
    }
    if(!strcmp(*arg, "logs")) {
        if(check_env("logs")) setenv("logs", "0", 1);
        else setenv("logs", "1", 1);

        return 1;
    }
    if(!strcmp(*arg, "sarg")) {
        if(check_env("sarg")) setenv("sarg", "0", 1);
        else setenv("sarg", "1", 1);

        return 1;
    }
    if(!strcmp(*arg, "mem")) {
        if(check_env("mem")) setenv("mem", "0", 1);
        else setenv("mem", "1", 1);

        return 1;
    }
    if(!strcmp(*arg, "bare")) {
        setenv("dbg", "0", 1);
        setenv("logs", "0", 1);
        setenv("sarg", "0", 1);
        setenv("mem", "0", 1);

        return 1;
    }
    if(!strcmp(*arg, "full")) {
        setenv("dbg", "1", 1);
        setenv("logs", "1", 1);
        setenv("sarg", "1", 1);
        // setenv("mem", "1", 1);

        return 1;
    }
    return 0;

}

void v_env(char ** argv) {
    for(int i = 0; argv[i]; i++) {
        if(!env(&argv[i])) {
            printf("Invalid environment setting: %s\n", argv[i]);
        }
    }
}

void what_args(char ** arg) {
    if(!check_env("sarg")) return;
    if(!arg) {printf("Clean.\n"); return;}
    if(check_env("logs")) printf("Args be: \n");
    for(int i = 0; arg[i] && *arg[i]; i++)
        printf("%d: \"%s\"\n", i + 1, arg[i]);
}

void make_env() {
    setenv("dbg", "0", 0);
    setenv("logs", "0", 0);
    setenv("sarg", "0", 0);
    setenv("mem", "0", 0);
    setenv("file", "-1", 0);
    setenv("io", "-1", 0);
    setenv("PATH", "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/TeX/texbin:/Users/abhay/.cabal/bin:/Users/abhay/.ghcup/bin:/Library/Frameworks/Python.framework/Versions/3.10/bin:/Users/abhay/.cargo/bin", 0);
    setenv("HOME", "/Users/abhay", 0);

}

void show_env() {
    printf("dbg = %s\n", getenv("dbg"));
    printf("logs = %s\n", getenv("logs"));
    printf("sarg = %s\n", getenv("sarg"));
    printf("mem = %s\n", getenv("mem"));
}
