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

    struct msg_info_request_t {

    };

    struct msg_info_response_t {
        authentication::server_info_t info;
    };

    struct msg_auth_request_t {
        authentication::credentials_t credentials;
    };

    struct msg_auth_response_t {
        authentication::ticket_t ticket;
    };

    struct msg_session_response_t {
        authentication::session_t session;
        network::ipv4_addr addr;
    };

    struct msg_login_request_t {
        authentication::session_t session;
    };

    struct msg_register_request_t {
        authentication::credentials_t credentials;
        authentication::serverID_t serverID;
    };

    struct msg_unregister_request_t {
        authentication::credentials_t credentials;
        authentication::serverID_t serverID;
    };




}

#endif
