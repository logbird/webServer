#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_BUFFER 1024 
#define SERV_PORT 8080
#define MAX_CONN 5
#define space(x) isspace((int)x)


void headers(int client);
void requestHandle(int reqFd);
int get_row_header(int fd, char *buf, size_t buf_size);

int main(int argc, char *argv[])
{
    int servFd, reqFd;
    struct sockaddr_in servaddr;
    pid_t pid;

    servFd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(servFd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        printf("server bind port %d failed!\n", SERV_PORT);
        exit(EXIT_FAILURE);
    }
    listen(servFd, MAX_CONN);
    printf("Listen %d, Max Connection:%d\n", SERV_PORT, MAX_CONN);

    while(1)
    {
        reqFd = accept(servFd, (struct sockaddr*)NULL, NULL);
        printf("accept\n");
        if(reqFd >= 0)
        {
            requestHandle(reqFd);
        }
        close(reqFd);
    }
    close(servFd);
    return 0;
}

void requestHandle(int reqFd)
{
    char buffer[MAX_BUFFER - 1];
    //int flags;
    int len = 0;

    //设置成非阻塞模式
    //flags = fcntl(reqFd, F_GETFL, 0);
    //fcntl(reqFd, F_SETFL, flags | O_NONBLOCK);

    headers(reqFd);
    while(get_row_header(reqFd, buffer, MAX_BUFFER))
    {
        printf("%s", buffer);
    }

    fflush(stdout);
    close(reqFd);
}

//get a line HTTP header, return 0 if reaches the end.
int get_row_header(int fd, char *buf, size_t buf_size)
{
    int c = 0, count = 0, i = 0;
    while(i < buf_size && c != '\n')
    {
        count = recv(fd, &c, 1, 0);
        if(count <= 0)
            break;
        buf[i] = c;
        i++;
    }
    buf[i] = '\0';
    if((i == 1 && buf[0] == '\n') || (i == 2 && buf[0] == '\r' && buf[1] == '\n'))
    {
        i = 0;
    }
    return i;
}

void headers(int client)
{
    char buf[MAX_BUFFER];

    strcpy(buf, "HTTP/1.1 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "<html><head></head><body>Hello World!</body></html>");
    send(client, buf, strlen(buf), 0);
}
