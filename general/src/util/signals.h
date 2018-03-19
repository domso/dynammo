#pragma once

namespace util {
    namespace signals {
        void default_handler(const int sig);
        
        void wait_for_sigint();
    };    
}
