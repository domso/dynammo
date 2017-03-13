#include "include/util/timestamp.h"

namespace util {
    namespace timestamp {
        
        const int UTIL_TIMER_START_TIME = time(nullptr);
        
        int now() {
            return time(nullptr);
        }
        
        int uptime() {
            return time(nullptr) - UTIL_TIMER_START_TIME;
        }
        
    }
}

