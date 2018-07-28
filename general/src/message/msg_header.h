#ifndef general_message_msg_header_h
#define general_message_msg_header_h

#include "src/util/error_code.h"
#include <cstdint>

namespace message {
    //______________________________________________________________________________________________________
    //
    // general msg header field types
    //______________________________________________________________________________________________________
    typedef uint8_t msg_type_t;
    typedef int8_t msg_status_t;
    typedef uint16_t msg_attr_t;
    
    namespace status {
        constexpr msg_status_t ok = 0;

        // process message later / Ressource is not available
        constexpr msg_status_t wait = -1;

        // drop message / do nothing / no send / no error
        constexpr msg_status_t close = 1;

        namespace error {
            constexpr msg_status_t unknown = 2;
            
            namespace auth {
                constexpr msg_status_t invalid_signature = 3;
                constexpr msg_status_t invalid_key = 4;
                constexpr msg_status_t invalid_connection = 5;
            }
            
            
            constexpr msg_status_t execution = 6;
            constexpr msg_status_t invalid_arguments = 7;
            constexpr msg_status_t access_denied = 8;
            constexpr msg_status_t timeout = 9;
            constexpr msg_status_t unavailable = 10;
            constexpr msg_status_t signing = 11;
            constexpr msg_status_t permission = 12;
        }
    }

    namespace option {
        constexpr msg_status_t clear = 0;
        constexpr msg_status_t no_encryption = 1;
        constexpr msg_status_t no_signing = 2;
        constexpr msg_status_t no_request_response_switch = 4;
    }
    
    constexpr int request_switch_mask = 0x80;
    //______________________________________________________________________________________________________
    //
    // simple header for any message
    //______________________________________________________________________________________________________
    struct msg_header_t {
        // type of the message (will be used by the msg_controller)
        msg_type_t msgType;
        // current message status
        msg_status_t status;
        // no usage currently
        int16_t attr;
    };
    //______________________________________________________________________________________________________
    //
    // general msg option
    //______________________________________________________________________________________________________
    typedef uint32_t msg_option_t;
}

#endif



