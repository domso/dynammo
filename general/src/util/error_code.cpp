#include "include/util/error_code.h"

namespace util {
    error_code::error_code() : code_(0) {

    }

    void error_code::set(const int code) {
        code_ = code;
    }

    bool error_code::setOnError(const int code, const bool constraint) {
        code_ = code * !constraint;
        return constraint;
    }

    int error_code::load() {
        int tmp = code_;
        code_ = 0;
        return tmp;
    }

    bool error_code::operator==(const int code) const {
        return code_ == code;
    }

    bool error_code::check(const int code) const {
        return code_ == code;
    }

    void error_code::clear() {
        code_ = 0;
    }
}
