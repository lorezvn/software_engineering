#include "main.h"

int micro_sleep(long usec) {
    struct timespec ts;
    int res;

    if (usec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    
    return res;  
}