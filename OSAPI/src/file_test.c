#include "../osapi.h"
// Server

int main() {
    int s1 = makesock();
    bindsock(s1, 7777, "localhost");
    int s2 = othersock(s1);
    FILE * f = fopen(__FILE__, "r");

    struct stat s;
    fstat(fileno(f), &s);

    off_t l = s.st_size;
    char * buf = malloc(l);
    read(fileno(f), buf, l);
    pushsock(s2, buf, l);

    int * out = pullsock(s2);
    if(*out != 1) {
        printf("Oops.\n");
    }

    close(s2);
    close(s1);
    free(out);
    free(buf);
    
}