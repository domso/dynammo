#ifndef general_util_semaphore
#define general_util_semaphore

#include "semaphore.h"

namespace util {
    //______________________________________________________________________________________________________
    //
    // wrapper for the posix semaphore
    //______________________________________________________________________________________________________
    class semaphore {
    public:
        //______________________________________________________________________________________________________
        //
        // Description:
        // - creates a new semaphore with 'value' as initial value
        // - see sem_init()
        // Parameter:
        // - value: init value
        //______________________________________________________________________________________________________
        semaphore(const int value = 0);
        ~semaphore();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to decrement the semaphore
        // - blocks until the semaphore is larger than zero
        // - see sem_wait
        // Return:
        // - true  | on success
        // - false | on error
        //______________________________________________________________________________________________________
        bool wait();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to decrement the semaphore
        // - if the semaphore equals zero, the function returns immediately
        // - see sem_trywait()
        // Return:
        // - true  | on success
        // - false | on error
        //______________________________________________________________________________________________________
        bool tryWait();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - tries to decrement the semaphore
        // - blocks until the semaphore is larger than zero or the given timeout had expired
        // - see sem_trywait()
        // Return:
        // - true  | on success
        // - false | on error or timeout
        //______________________________________________________________________________________________________
        bool timedWait(double sec2wait);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - increments the semaphore
        // - see sem_post()
        // Parameter:
        // - ignored: the value will be ignored!
        // Return:
        // - true  | on success
        // - false | on error
        //______________________________________________________________________________________________________
        bool operator++(int ignored);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - increments the semaphore
        // - see sem_post()
        // Return:
        // - true  | on success
        // - false | on error
        //______________________________________________________________________________________________________
        bool post();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - see sem_getvalue()
        // Return:
        // - current value of the semaphore (>=0) | on success
        // - -1                                   | on error
        //______________________________________________________________________________________________________
        int getValue();

        semaphore(const semaphore& that) = delete;
        void operator=(semaphore& that) = delete;
    private:

        sem_t posixSemaphore_;
    };
}

#endif
