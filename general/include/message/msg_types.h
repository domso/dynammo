#ifndef general_msg_type_h
#define general_msg_type_h

#include <cstdint>
#include "include/message/msg_header.h"
#include "include/authentication/types.h"

#define MSG_TYPE_INFO 1
#define MSG_TYPE_LOGIN 2
#define MSG_TYPE_LOGOUT 3
#define MSG_TYPE_LOOKUP 4

namespace message {
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    struct msg_info_request_t {

    };

    struct msg_info_response_t {

        authentication::server_info_t info;
    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    struct msg_login_request_t {

        authentication::credentials_t credentials;
    };

    struct msg_login_response_t {

        authentication::ticket_t ticket;
    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    struct msg_logout_request_t {

        authentication::identification_t identification;
    };

    struct msg_logout_response_t {

    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    struct msg_lookup_request_t {

        authentication::identification_t identification;
        authentication::ticket_t ticket;
    };

    struct msg_lookup_response_t {

        authentication::accountID_t accountID;
    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif
