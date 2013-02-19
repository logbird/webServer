#include <stdio.h>

#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "core.h"


void php_cgi(char const *path, int fd);

int main(int argc, char *argv[])
{
    server_init();
    server_start();
    server_listen();
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


