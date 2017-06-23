#ifndef general_util_thread_controller_h
#define general_util_thread_controller_h

#include <thread>

namespace util {
    //______________________________________________________________________________________________________
    //
    // simple controller to start and detach threads
    // each thread calls T::exec on an instance of T
    //______________________________________________________________________________________________________
    template <typename T>
    class thread_controller {
    public:

        thread_controller() : internalInstance_(false), data_(nullptr) {

        }

        thread_controller(thread_controller& that) = delete;

        //______________________________________________________________________________________________________
        //
        // Description:
        // - calls arg->execute()
        // - if arg is internal, the instance is deleted afterwards
        // Parameter:
        // - arg: pointer to a valid instance of T or nullptr
        //______________________________________________________________________________________________________
        static void mainThread(T* arg, bool internalInstance) {
            if (arg != nullptr) {
                arg->execute();

                if (internalInstance) {
                    delete arg;
                }
            }
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - deletes old internal instances of T and sets a new object as the current internal instance
        // Return:
        // - pointer to new instance
        //______________________________________________________________________________________________________
        T* createNew() {
            if (data_ != nullptr && internalInstance_) {
                delete data_;
            }

            data_ = new T();
            internalInstance_ = true;
            return data;
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - deletes old internal instances of T and sets 'data' as the current external instance
        // Parameter:
        // - data: pointer to a valid instance of T
        //______________________________________________________________________________________________________
        void setNew(T* data) {
            if (data_ != nullptr && internalInstance_) {
                delete data_;
            }

            data_ = data;
            internalInstance_ = false;
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - starts a new thread with a local instance of T
        // - detaches the thread afterwards (see std::thread::detach())
        // - sets the current instance to nullptr
        //______________________________________________________________________________________________________
        void start() {
            mainThreadHandle_ = std::thread(mainThread, data_, internalInstance_);
            mainThreadHandle_.detach();
            data_ = nullptr;
        }
        //______________________________________________________________________________________________________
        //
        // Return:
        // - current local instance of T
        //______________________________________________________________________________________________________
        T* data() {
            return data;
        }
        //______________________________________________________________________________________________________
        //
        // Return:
        // - internal thread handler
        //______________________________________________________________________________________________________
        std::thread& getHandle() {
            return mainThreadHandle_;
        }

    private:
        bool internalInstance_;
        T* data_;
        std::thread mainThreadHandle_;
    };
}

#endif

