#ifndef ERR_H_
#define ERR_H_
#include <signal.h>
#include <errno.h>

#define ERR(source) (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), \
                     perror(source), kill(0, SIGKILL),               \
                     exit(EXIT_FAILURE))

#endif // ERR_H_