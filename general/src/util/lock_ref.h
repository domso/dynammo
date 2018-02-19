#ifndef general_util_lock_ref_h
#define general_util_lock_ref_h

namespace util {
    // Holds ownership by locking the mutex provided by T::mutex()
    // The destructor of T needs to lock the mutex!
    template <typename T>
    class locked_ref {
    public:
        locked_ref(T& data) : m_data(data), m_lock(data.mutex()) {}
        locked_ref(const locked_ref& copy) = delete;
        locked_ref(locked_ref&& move) : m_data(move.m_data), m_lock(std::move(move.m_lock)) {}

        T* operator->() {
            return &m_data;
        }
    private:
        T& m_data;
        std::unique_lock<std::mutex> m_lock;
    };
}

#endif
