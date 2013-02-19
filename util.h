#ifndef INC_UTIL
#define INC_UTIL 1


#define ispace(x) isspace((int)x)

int read_to_space(char *ret, char *buffer, int size, unsigned int *offset);

void error(char const *err);
#endif


