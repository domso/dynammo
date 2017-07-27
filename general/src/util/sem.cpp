#include "src/util/sem.h"

#include "assert.h"
#include "time.h"


namespace util {

    semaphore::semaphore(const int value) {
        int m_semaphoreinit_result = sem_init(&m_posixSemaphore, 0, value);
        assert(m_semaphoreinit_result == 0 && "Could not create semaphore");
    }

    semaphore::~semaphore() {
        sem_destroy(&m_posixSemaphore);
    }

    bool semaphore::wait() {
        return sem_wait(&m_posixSemaphore) == 0;
    }

    bool semaphore::tryWait() {
        return sem_trywait(&m_posixSemaphore) == 0;
    }

    bool semaphore::timedWait(double sec2wait) {

        struct timespec ts;

        if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
            ts.tv_sec += (int)sec2wait;
            ts.tv_nsec += (double)(sec2wait - (int)sec2wait) * 1000000000;
            ts.tv_sec += ts.tv_nsec >= 1000000000;
            ts.tv_nsec -= (ts.tv_nsec >= 1000000000) * 1000000000;

            return sem_timedwait(&m_posixSemaphore, &ts) == 0;
        }

        return false;
    }

    bool semaphore::operator++(int) {
        return sem_post(&m_posixSemaphore) == 0;
    }

    bool semaphore::post() {
        return sem_post(&m_posixSemaphore) == 0;
    }

    int semaphore::getValue() {
        int result;
        return -1 + (sem_getvalue(&m_posixSemaphore, &result) == 0) * (result + 1);
    }
}
