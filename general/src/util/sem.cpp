#include "include/util/sem.h"

#include "assert.h"
#include "time.h"


namespace util {

    semaphore::semaphore(const int value) {
        int semaphore_init_result = sem_init(&posixSemaphore_, 0, value);
        assert(semaphore_init_result == 0 && "Could not create semaphore");
    }

    semaphore::~semaphore() {
        sem_destroy(&posixSemaphore_);
    }

    bool semaphore::wait() {
        return sem_wait(&posixSemaphore_) == 0;
    }

    bool semaphore::tryWait() {
        return sem_trywait(&posixSemaphore_) == 0;
    }

    bool semaphore::timedWait(double sec2wait) {

        struct timespec ts;

        if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
            ts.tv_sec += (int)sec2wait;
            ts.tv_nsec += (double)(sec2wait - (int)sec2wait) * 1000000000;
            ts.tv_sec += ts.tv_nsec >= 1000000000;
            ts.tv_nsec -= (ts.tv_nsec >= 1000000000) * 1000000000;

            return sem_timedwait(&posixSemaphore_, &ts) == 0;
        }

        return false;
    }

    bool semaphore::operator++(int) {
        return sem_post(&posixSemaphore_) == 0;
    }

    bool semaphore::post() {
        return sem_post(&posixSemaphore_) == 0;
    }

    int semaphore::getValue() {
        int result;
        return -1 + (sem_getvalue(&posixSemaphore_, &result) == 0) * (result + 1);
    }
}
