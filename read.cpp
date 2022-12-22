#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include "common.h"

int main(int argc, char *argv[])
{
    usleep(10000);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *shmpath = argv[1];
    int fd = shm_open(shmpath, O_RDWR, 0);
    if (fd == -1)
        errExit("shm_open");
        
    struct shmbuf *shmp = (struct shmbuf *)mmap(NULL, sizeof(*shmp),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap"); 

    double local_time_sum = 0;


    for (size_t ts = 0; ts < TEST_ROUND; ++ts) {
        while (shmp->idx != ts) continue;  
        if (ts > 0) {   
            local_time_sum += std::chrono::system_clock::now().time_since_epoch().count()
            - shmp->local_time;
        }
    }
    std::cout << local_time_sum / (TEST_ROUND - 1) << std::endl;
    
    exit(EXIT_SUCCESS);
}
 