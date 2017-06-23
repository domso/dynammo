#ifndef general_util_connect_thread
#define general_util_connect_thread

#include <atomic>
#include "include/util/msg_queue.h"
#include "include/util/sem.h"


namespace util {
    //______________________________________________________________________________________________________
    //
    // simple execution pipeline
    // INPUT --> execute() --> OUTPUT
    //______________________________________________________________________________________________________
    template <typename T>
    class execution_pipeline {
    public:
        execution_pipeline() : running_(true), close_(false), semaphore_(0), inputQueue_(1024), outputQueue_(1024) {

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
            while (running_.load()) {
                semaphore_.wait();

                if (inputQueue_.isAvailable<T>() && outputQueue_.isFree<T>()) {
                    auto r = inputQueue_.read<T>();
                    auto w = outputQueue_.write<T>();

                    if (r.obj != nullptr && w.obj != nullptr) {
                        r.obj->execute();
                        *w.obj = *r.obj;
                    }

                }
            }

            close_.store(true);
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - closes the pipeline
        // - blocks until the execute()-call has finished
        //______________________________________________________________________________________________________
        void close() {
            running_.store(false);
            notify();

            while (!close_.load()) {
                // wait for thread to terminate
            }
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - wakes up the thread calling execution_pipeline::execute()
        //______________________________________________________________________________________________________
        void notify() {
            semaphore_.post();
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
            return inputQueue_.write<T>(n);
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
            return outputQueue_.read<T>(n);
        }

        execution_pipeline(const execution_pipeline& that) = delete;
        void operator=(int) = delete;
    private:

        util::msg_queue inputQueue_;
        util::msg_queue outputQueue_;

        std::atomic<bool> running_;
        std::atomic<bool> close_;

        util::semaphore semaphore_;
    };
}

#endif


