#include <errno.h>
#include <err.h>
#include <sysexits.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libgen.h>

#include "sqserv.h"

char *prog_name;

void usage() {
    printf("usage: %s [-p <port>]\n", prog_name);
    printf("\t-p <port> specify alternate port\n");
}

int main(int argc, char* argv[]) {

    long port = DEFAULT_PORT;
    struct sockaddr_in sin;
    int fd;
    socklen_t sin_len;
    size_t  data_len;
    ssize_t len;
    char *data, *end;
    long ch;

    /* get options and arguments */
    prog_name = strdup(basename(argv[0]));
    while ((ch = getopt(argc, argv, "?hp:")) != -1) {
        switch (ch) {
            case 'p': port = strtol(optarg, (char **)NULL, 10);break;
            case 'h':
            case '?':
            default: usage(); return 0;
        }
    }
    argc -= optind;
    argv += optind;

    if (argc != 0) {
        usage();
        return EX_USAGE;
    }

    printf("listening on port %d\n", (int)port);

    /* get room for data */
    data_len = BUFFER_SIZE;
    if ((data = (char *) malloc(data_len)) < 0) {
        err(EX_SOFTWARE, "in malloc");
    }

    /* create and bind a socket */
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        free(data);
        err(EX_SOFTWARE, "in socket");
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY; // rather memcpy to sin.sin_addr
    sin.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        free(data);
        close(fd);
        err(EX_SOFTWARE, "in bind");
    }

    /* receive data */
    sin_len = sizeof(sin);
    len = recvfrom(fd, data, data_len, 0, (struct sockaddr *)&sin, &sin_len);
    if (len < 0) {
        free(data);
        close(fd);
        err(EX_SOFTWARE, "in recvfrom");
    }

    printf("got '%s' from IP address %s port %d\n", data, inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

    ch = strtol(data, &end, 10);

    switch (errno) {
        case EINVAL: err(EX_DATAERR, "not an integer");
        case ERANGE: err(EX_DATAERR, "out of range");
        default: if (ch == 0 && data == end) errx(EX_DATAERR, "no value");  // Linux returns 0 if no numerical value was given
    }

    /* send data */
    printf("integer value: %ld\n", ch);

    /* cleanup */
    free(data);
    close(fd);

    return EX_OK;
}
