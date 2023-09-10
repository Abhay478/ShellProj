#include "../osapi.h"

int main() {
    int s = makesock();
    connectsock(s, 7777, "127.0.0.1");

    char * buf = pullsock(s);

    printf("%s\n", buf);

    int x = 1;
    pushsock(s, &x, sizeof(int));
}