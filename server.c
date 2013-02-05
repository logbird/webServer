#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_BUFFER 1024 
#define METHOD_SIZE 4 
#define SERV_PORT 8080
#define MAX_CONN 5
#define space(x) isspace((int)x)


void requestHandle(int reqFd);
int get_row_header(int fd, char *buf, size_t buf_size);
void php_cgi(char const *path, int fd);
int sendHeader(int fd, char const *header);
void headers(int client);
//read to space
int read_to_space(char *ret, char *buffer, int size, unsigned int *offset);

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
        if(reqFd >= 0)
        {
            pid = fork();
            if(pid == 0)
            {
                requestHandle(reqFd);
                exit(EXIT_SUCCESS);
            }else if(pid > 0)
            {
                wait();
            }
        }
        close(reqFd);
    }
    close(servFd);
    return 0;
}

void requestHandle(int reqFd)
{
    char buffer[MAX_BUFFER];
    char method[METHOD_SIZE + 1];
    char header[MAX_BUFFER];
    char script[MAX_BUFFER] = ".";
    int len = 0, tmp_len = 0, i = 0;
    int offset = 0;

    //First Row
    len = get_row_header(reqFd, buffer, MAX_BUFFER);
    //Get METHOD
    tmp_len = read_to_space(method, buffer, METHOD_SIZE, &offset);
    //Get Script
    read_to_space(script+1, buffer, len - tmp_len, &offset);

    //!!!!!!!!!!!!!!!!!!!!!!!!!---处理404返回文件大小等---!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(access(script, 0) != 0)
    {
        sendHeader(reqFd, "HTTP/1.1 404 NOT FOUND\r\n");
        sendHeader(reqFd, "Content-Type: text/html; Charset=UTF-8\r\n");
        sendHeader(reqFd, "\r\n");
        sendHeader(reqFd, "<html><head><title>找不到网页</title></head><body>您要找的网页不存在</body></html>");

    }else
    {
        headers(reqFd);
        php_cgi(script, reqFd);
    }

    fflush(stdout);
    close(reqFd);
}

void php_cgi(char const *path, int fd)
{
    char script[500];
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    strcpy(script, "SCRIPT_FILENAME=");
    strcat(script, path);
    putenv("GATEWAY_INTERFACE=CGI/1.1");
    putenv(script);
    putenv("QUERY_STRING=");
    putenv("REQUEST_METHOD=GET");
    putenv("REDIRECT_STATUS=true");
    putenv("SERVER_PROTOCOL=HTTP/1.1");
    putenv("REMOTE_HOST=127.0.0.1");
    execl("/usr/local/php-dev/bin/php-cgi", "php-cgi", NULL);
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
    sendHeader(client, "HTTP/1.1 200 OK\r\n");
    sendHeader(client, "Content-Type: text/html\r\n");
}

int sendHeader(int fd, char const *header)
{
    char buf[MAX_BUFFER];
    if(buf == NULL)
    {
        return -1;
    }
    sprintf(buf, "%s", header);
    send(fd, buf, strlen(buf), 0);
    return 1;
}

int read_to_space(char *ret, char *buffer, int size, unsigned int *offset)
{
    int i = 0;
    int location = (int)*offset;
    if(location > 0)
    {
        location++;
    }else
    {
        location = 0;
    }
    while(location < size && !space(buffer[location]))
    {
        ret[i] = buffer[location];
        i++;
        location++;
    }
    ret[location] = '\0';
    *offset = location;
    return i;
}

