#ifndef general_util_wait_lock_h
#define general_util_wait_lock_h

#include <chrono>
#include <mutex>
#include <condition_variable>

namespace util {
    namespace wait_lock {
        //______________________________________________________________________________________________________
        //
        // wrapper for an instance of T
        // containing all necessary data for locking
        // NEVER access the fields directly!
        //______________________________________________________________________________________________________
        template <typename T>
        struct context_t {
            T data;
            bool interrupt = false;
            std::mutex mutex;
            std::condition_variable cond;
        };
        //______________________________________________________________________________________________________
        //
        // ownership-class for read-access
        // needs to be destroyed to release the ressource
        //______________________________________________________________________________________________________
        template <typename T>
        class read {
        public:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - locks the given ressource for read access
            // - if the lock is currently locked by any access-type (!), this call will block the execution until the ressource is freed
            // Parameter:
            // - context: the wrapped ressource
            //______________________________________________________________________________________________________
            read(context_t<T>& context) : m_lock(context.mutex), m_data(context.data) {

            }

            ~read() {

            }
            //______________________________________________________________________________________________________
            //
            // Return: a reference to the actual protected ressource
            //______________________________________________________________________________________________________
            const T& data() const {
                return m_data;
            }
            //______________________________________________________________________________________________________
            //
            // Return: an pointer to the actual protected ressource
            //______________________________________________________________________________________________________
            const T* const operator ->() const {
                return &m_data;
            }
        private:
            // reference to the actual protected ressource
            T& m_data;
            // internal representation of the lock
            std::unique_lock<std::mutex> m_lock;
        };
        //______________________________________________________________________________________________________
        //
        // ownership-class for write-access
        // needs to be destroyed to release the ressource and wake up all waiting threads (see wait)
        //______________________________________________________________________________________________________
        template <typename T>
        class write {
        public:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - locks the given ressource for write access
            // - if the lock is currently locked by any access-type (!), this call will block the execution until the ressource is freed
            // Parameter:
            // - context: the wrapped ressource
            //______________________________________________________________________________________________________
            write(context_t<T>& context) : m_lock(context.mutex), m_context(context) {

            }
            //______________________________________________________________________________________________________
            //
            // Description:
            // - wake up all waiting threads (see wait)
            //______________________________________________________________________________________________________
            ~write() {
                m_context.interrupt = true;
                m_context.cond.notify_all();
            }
            //______________________________________________________________________________________________________
            //
            // Return: a reference to the actual protected ressource
            //______________________________________________________________________________________________________
            T& data() {
                return m_context.data;
            }
            //______________________________________________________________________________________________________
            //
            // Return: an pointer to the actual protected ressource
            //______________________________________________________________________________________________________
            T* operator ->() {
                return &m_context.data;
            }
        private:
            // reference to the protected ressource
            context_t<T>& m_context;
            // internal representation of the lock
            std::unique_lock<std::mutex> m_lock;
        };
        //______________________________________________________________________________________________________
        //
        // ownership-class for write-access after an update (any write-access finished before or while waiting)
        // needs to be destroyed to release the ressource
        //______________________________________________________________________________________________________
        template <typename T>
        class wait {
        public:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - locks the given ressource for write access
            // - if the lock is currently locked by any access-type (!), this call will block the execution until the ressource is freed
            // - waits until the ressource was updated by any write-access and resets the update-status afterwards
            // Parameter:
            // - context: the wrapped ressource
            // - timeoutSec: maximal wait-time
            //______________________________________________________________________________________________________
            wait(context_t<T>& context, double timeoutSec = 0.1) : m_lock(context.mutex), m_data(context.data) {
                while (!context.interrupt) {
                    std::chrono::duration<double> duration(timeoutSec);
                    m_valid = context.cond.wait_for(m_lock, duration) == std::cv_status::no_timeout;

                    if (!m_valid) {
                        return;
                    }
                }

                context.interrupt = false;
            }

            ~wait() {

            }
            //______________________________________________________________________________________________________
            //
            // Return:
            // - true  | an update was performed
            // - false | a timeout occured while waiting
            //______________________________________________________________________________________________________
            bool isValid() {
                return m_valid;
            }
            //______________________________________________________________________________________________________
            //
            // Return: a reference to the actual protected ressource
            //______________________________________________________________________________________________________
            T& data() {
                return m_data;
            }
            //______________________________________________________________________________________________________
            //
            // Return: an pointer to the actual protected ressource
            //______________________________________________________________________________________________________
            T* operator ->() {
                return &m_data;
            }
        private:
            // reference to the actual protected ressource
            T& m_data;
            // update-status
            bool m_valid;
            // internal representation of the lock
            std::unique_lock<std::mutex> m_lock;
        };
    }
}

#endif

