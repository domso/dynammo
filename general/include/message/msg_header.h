#ifndef general_message_msg_header_h
#define general_message_msg_header_h

#include <cstdint>
// drop message / do nothing / no send
#define MSG_STATUS_CLOSE 0

#define MSG_STATUS_OK 1
#define MSG_STATUS_WARNING 2

#define MSG_STATUS_AUTH_ERROR -1
#define MSG_STATUS_DATA_ERROR -2
#define MSG_STATUS_UNKOWN_ERROR -3

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
}

#endif
