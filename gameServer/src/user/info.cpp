#include "src/user/info.h"

user::info::info() {
}

user::info::~info() {
    std::lock_guard<std::mutex> lg(m_mutex);
}

std::mutex& user::info::mutex() {
    return m_mutex;
}


