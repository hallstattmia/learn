#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

#define BUF_SIZE 1024   /* Maximum size for exchanged string */
#define TEST_ROUND 100
/* Define a structure that will be imposed on the shared
    memory object */

struct shmbuf {
    int idx;
    int64_t local_time;
};
