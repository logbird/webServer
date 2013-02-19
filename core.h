#ifndef INC_CORE
#define INC_CORE 1

#define SERV_PORT 8080
#define MAX_CONN 5

void server_init();
void server_start();
void server_listen();
void server_handle(int reqFd);
void server_shutdown();

void sig_handle(int sigtype);
#endif
