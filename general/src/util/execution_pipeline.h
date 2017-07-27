#ifndef general_util_connect_thread
#define general_util_connect_thread

#include <atomic>
#include "src/util/msg_queue.h"
#include "src/util/sem.h"


namespace util {
    //______________________________________________________________________________________________________
    //
    // simple execution pipeline
    // INPUT --> execute() --> OUTPUT
    //______________________________________________________________________________________________________
    template <typename T>
    class execution_pipeline {
    public:
        execution_pipeline() : m_running(true), m_close(false), m_semaphore(0), m_inputQueue(1024), m_outputQueue(1024) {

        }

        ~execution_pipeline() {
            close();
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - reads an instance of T from the input-queue, executes T::execute() on this instance
        // - afterwards the object will be written to the output-queue
        //______________________________________________________________________________________________________
        void execute() {
            while (m_running.load()) {
                m_semaphore.wait();

                if (m_inputQueue.isAvailable<T>() && m_outputQueue.isFree<T>()) {
                    auto r = m_inputQueue.read<T>();
                    auto w = m_outputQueue.write<T>();

                    if (r.obj != nullptr && w.obj != nullptr) {
                        r.obj->execute();
                        *w.obj = *r.obj;
                    }

                }
            }

            m_close.store(true);
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - closes the pipeline
        // - blocks until the execute()-call has finished
        //______________________________________________________________________________________________________
        void close() {
            m_running.store(false);
            notify();

            while (!m_close.load()) {
                // wait for thread to terminate
            }
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - wakes up the thread calling execution_pipeline::execute()
        //______________________________________________________________________________________________________
        void notify() {
            m_semaphore.post();
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - reserve 'n' instances of T in the input-queue
        // - execution_pipeline::notify() needs to be calld afterwards
        // Parameter:
        // - n: number of requested instances
        //______________________________________________________________________________________________________
        util::msg_queue::writer<T> push(const int n = 1) {
            return m_inputQueue.write<T>(n);
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - loads 'n' instances of T from the output-queue
        // - execution_pipeline::notify() needs to be calld afterwards
        // Parameter:
        // - n: number of requested instances
        //______________________________________________________________________________________________________
        util::msg_queue::reader<T> pop(const int n = 1) {
            return m_outputQueue.read<T>(n);
        }

        execution_pipeline(const execution_pipeline& that) = delete;
        void operator=(int) = delete;
    private:

        util::msg_queue m_inputQueue;
        util::msg_queue m_outputQueue;

        std::atomic<bool> m_running;
        std::atomic<bool> m_close;

        util::semaphore m_semaphore;
    };
}

#endif


