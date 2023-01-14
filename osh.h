#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#define N_ARGS 10
#define ARG_LEN 50
#define check_env(e) atoi(getenv(e))
#define over(a, b) for(int i = a; i < b; i++)
#define rover(a, b) for(int i = b - 1; i >= a; i--)

#define iter(a) over(0, a)
#define riter(a) rover(0, a)

#define MEMORY 20

typedef struct exe {
    char ** args;
    char * cmd;
    int fd[2];
    struct exe * pipee;
    int fgbg;
} Exec;

typedef struct hi {
    int capac;
    Exec ** hist;
} History;

int set_path(char ** arg);

int env(char ** arg);
void v_env(char ** argv);
void what_exec(Exec * the);
void make_env();

void prompt(char ** cmd);

Exec * parse(char * cmd, int bg);

void spawn(Exec * the);

Exec * pop(History * the);
History * init();
void push(History * the, Exec * done);
Exec * peep(History * the, int age);

Exec * build_exec(char ** cmd, int bg);
void lore(History * the);