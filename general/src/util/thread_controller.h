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

        thread_controller() : m_internalInstance(false), m_data(nullptr) {

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
            if (m_data != nullptr && m_internalInstance) {
                delete m_data;
            }

            m_data = new T();
            m_internalInstance = true;
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
            if (m_data != nullptr && m_internalInstance) {
                delete m_data;
            }

            m_data = data;
            m_internalInstance = false;
        }
        //______________________________________________________________________________________________________
        //
        // Description:
        // - starts a new thread with a local instance of T
        // - detaches the thread afterwards (see std::thread::detach())
        // - sets the current instance to nullptr
        //______________________________________________________________________________________________________
        void start() {
            m_mainThreadHandle = std::thread(mainThread, m_data, m_internalInstance);
            m_mainThreadHandle.detach();
            m_data = nullptr;
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
            return m_mainThreadHandle;
        }

    private:
        bool m_internalInstance;
        T* m_data;
        std::thread m_mainThreadHandle;
    };
}

#endif

