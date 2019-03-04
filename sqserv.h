#define DEFAULT_PORT 5000
#define BUFFER_SIZE 128

#ifndef FD_COPY
#define FD_COPY(orig,dest) memcpy((dest),(orig),sizeof(*(dest)))
#endif