#ifndef general_util_notificator_h
#define general_util_notificator_h

namespace util {
    //______________________________________________________________________________________________________
    //
    // calls T::notify() on destruction
    //______________________________________________________________________________________________________
    template <typename T>
    class notificator {
    public:
        notificator(const T* notifier) : notifier(notifier) {

        }

        ~notificator() {
            m_notifier->notify();
        }
    private:
        T* m_notifier;
    };

}

#endif
