#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include "common.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *shmpath = argv[1];

    int fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR,
                                 S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("shm_open");

    if (ftruncate(fd, sizeof(struct shmbuf)) == -1)
        errExit("ftruncate");

    usleep(10000);
    /* Map the object into the caller's address space. */

    struct shmbuf *shmp = (struct shmbuf *)mmap(NULL, sizeof(*shmp),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");
    for (size_t ts = 0; ts < TEST_ROUND; ++ts) {
        shmp->idx = ts;
        const std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
        shmp->local_time = now.time_since_epoch().count();
        usleep(10000);
    }

    shm_unlink(shmpath);

    exit(EXIT_SUCCESS);
}
 