#ifndef INC_HTTP
#define INC_HTTP 1

#define MAX_BUFFER 1024 
#define METHOD_SIZE 4 

int get_row_header(int fd, char *buf, size_t buf_size);
int sendHeader(int fd, char const *header);
void http_parse(int reqFd);

#endif

