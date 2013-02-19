#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include <signal.h>

#include "core.h"
#include "util.h"
#include "http.h"

static int connection_id = 0;


void server_init()
{
    //deamon handdle
    //signal handdle
    signal(SIGINT, &sig_handle);
    signal(SIGQUIT, &sig_handle);
    signal(SIGTERM, &sig_handle);
}

void server_start()
{
    struct sockaddr_in servaddr;
    connection_id = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(connection_id, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        printf("server bind port %d failed!\n", SERV_PORT);
        exit(EXIT_FAILURE);
    }
}

void server_listen()
{
    int reqFd;
    pid_t pid;
    listen(connection_id, MAX_CONN);
    printf("Listen %d, Max Connection:%d\n", SERV_PORT, MAX_CONN);

    while(1)
    {
        reqFd = accept(connection_id, (struct sockaddr*)NULL, NULL);
        if(reqFd >= 0)
        {
            pid = fork();
            if(pid == 0)
            {
                server_handle(reqFd);
                exit(EXIT_SUCCESS);
            }else if(pid > 0)
            {
                wait();
            }
        }
        close(reqFd);
    }
}

void server_handle(int reqFd)
{
    printf("Visited!\n");
    close(reqFd);
}

void server_shutdown()
{
    close(connection_id);
    printf("bye!!!\n");
    exit(EXIT_SUCCESS);
}

void sig_handle(int sigtype)
{
    server_shutdown();
}
