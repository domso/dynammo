#include "src/util/error_code.h"

namespace util {
    error_code::error_code() : m_code(0) {

    }

    void error_code::set(const int code) {
        m_code = code;
    }

    bool error_code::setOnError(const int code, const bool constraint) {
        m_code = code * !constraint;
        return constraint;
    }

    int error_code::load() {
        int tmp = m_code;
        m_code = 0;
        return tmp;
    }

    bool error_code::operator==(const int code) const {
        return m_code == code;
    }

    bool error_code::check(const int code) const {
        return m_code == code;
    }

    void error_code::clear() {
        m_code = 0;
    }
}
