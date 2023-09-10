// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include<sys/socket.h>
// #include<arpa/inet.h>
// #include<unistd.h>
// #include <errno.h>

#include "osapi.h"

/// @brief Init socket
/// @return File descriptor
int makesock() {
    int soc1 = socket(AF_INET, SOCK_STREAM, 0);
    if(chkpt(soc1))
        printf("Socket made: %d\n", soc1);
    return soc1;
}

/// @brief Wrapper around listen()
/// @param socket 
/// @param port 
/// @param IP 
void bindsock(int socket, int port, char * IP){
    struct sockaddr_in s;
    s.sin_family = AF_INET;
    s.sin_addr.s_addr = htonl(INADDR_ANY);
    s.sin_port = htons(port);
    
    if (chkpt(bind(socket, (struct sockaddr *)&s, sizeof(s)))) {
        listen(socket, 3);
        printf("Listening at %s:%d\n", IP, port);
    }
}

/// @brief Wrapper around accept()
/// @param socket: Current socket
/// @return: Connected socket
int othersock(int socket)
{
    struct sockaddr_in client;
    int l = sizeof(client);
    int out = -1;
    if(chkpt(out = accept(socket, (struct sockaddr *)&client, (socklen_t *)&l)))
        printf("Connection established.\n");
    
    return out;
}

/// @brief Wrapper around send()
/// @param socket: Other socket 
/// @param message
/// @param len 
void pushsock(int socket, void * message, int len) 
{
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;

    if(chkpt(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv))))
        send(socket, message, len, 0);
    
}

/// @brief Wrapper around recv()
/// @param socket: Other socket.
/// @return Buffer containing transmitted/received data.
void * pullsock(int socket)
{
    void * out = calloc(1600, sizeof(char));
    chkpt(recv(socket, out, 1600, 0));
    return out;

}

/// @brief Client-side, wrapper around connect()
/// @param socket: Current socket.
/// @param port: Server-side port.
/// @param IP: Server IP.
void connectsock(int socket, int port, char * IP){
    struct sockaddr_in s;
    s.sin_family = AF_INET;
    s.sin_addr.s_addr = inet_addr(IP);
    s.sin_port = htons(port);
    
    if (chkpt(connect(socket, (struct sockaddr *)&s, sizeof(struct sockaddr_in))))
        printf("Successfully connected to %s:%d\n", IP, port);
}
