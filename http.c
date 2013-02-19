#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "util.h"
#include "http.h"
#include "hash.h"

hashStr *http_headers;

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

int sendHeader(int fd, char const *header)
{
    char buf[MAX_BUFFER];
    if(buf == NULL)
    {
        return -1;
    }
    sprintf(buf, "%s", header);
    if(send(fd, buf, strlen(buf), 0) == -1)
    {
        error("Send Err!");
    }
    return 1;
}

void http_parse(int reqFd)
{
    char buffer[MAX_BUFFER];
    char method[METHOD_SIZE + 1];
    char header[MAX_BUFFER];
    char script[MAX_BUFFER] = ".";
    int len = 0, tmp_len = 0, i = 0;
    int offset = 0;
    struct stat script_st;
    http_headers = create_hash();

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
        sendHeader(reqFd, "HTTP/1.1 200 OK\r\n");
        sendHeader(reqFd, "Content-Type: text/html; Charset=UTF-8\r\n");
        //Content-Length
        if(stat(script, &script_st) == -1)
        {
            error("Err1");
        }
        sprintf(header, "Content-Length: %d\r\n", (int)script_st.st_size);
        sendHeader(reqFd, header);
    }
}
