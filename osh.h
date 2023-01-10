#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#define N_ARGS 10
#define ARG_LEN 20
#define check_env(e) atoi(getenv(e))
#define over(a, b) for(int i = a; i < b; i++)
#define rover(a, b) for(int i = b - 1; i >= a; i--)

#define iter(a) over(0, a)
#define riter(a) rover(0, a)

int set_path(char ** arg);

int env(char ** arg);
void v_env(char ** argv);
void what_args(char ** arg);
void make_env();

void prompt(char ** cmd);

char ** parse(char * cmd, int * bg);

void spawn(char ** arg, int bg);