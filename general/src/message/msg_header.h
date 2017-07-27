#ifndef general_message_msg_header_h
#define general_message_msg_header_h

#include "src/util/error_code.h"
#include <cstdint>

#define MSG_STATUS_OK 0

// process message later / Ressource is not available
#define MSG_STATUS_WAIT -1

// drop message / do nothing / no send / no error
#define MSG_STATUS_CLOSE 1

#define MSG_STATUS_UNKOWN_ERROR 2
#define MSG_STATUS_AUTH_ERROR 3
#define MSG_STATUS_EXECUTION_ERROR 4
#define MSG_STATUS_INVALID_ARGUMENTS 5
#define MSG_STATUS_ACCESS_DENIED 6
#define MSG_STATUS_TIMEOUT 7
#define MSG_STATUS_UNAVAILABLE 8

#define MSG_OPTION_CLEAR 0
#define MSG_OPTION_NO_ENCRYPTION 1
#define MSG_OPTION_NO_SIGNING 2
#define MSG_OPTION_NO_REQUEST_RESPONSE_SWITCH 4

#define MSG_HEADER_TYPE_REQUEST_SWITCH_MASK 0x80

namespace message {
    //______________________________________________________________________________________________________
    //
    // general msg header field types
    //______________________________________________________________________________________________________
    typedef uint8_t msg_type_t;
    typedef int8_t msg_status_t;
    typedef uint16_t msg_attr_t;
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
