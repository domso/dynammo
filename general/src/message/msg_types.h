#ifndef general_msg_type_h
#define general_msg_type_h

#include <cstdint>
#include "src/message/msg_header.h"
#include "src/authentication/types.h"

namespace message {
    
    
    namespace types {
        enum states {
          ready, wait, recv  
        };
    }

    struct msg_info_request_t {

    };

//     struct msg_info_response_t {
//         authentication::server_info_t info;
//     };
// 
//     struct msg_auth_request_t {
//         authentication::accountID_t accountID;
//     };
// 
//     struct msg_auth_response_t {
//         authentication::accountID_t accountID;
//     };
// 
//     struct msg_login_request_t {
//         authentication::ticket_t ticket;
//         authentication::session_t session;
//     };
//     
//     struct msg_logout_request_t {
//         authentication::session_t session;
//     };
// 
//     struct msg_register_request_t {
//         authentication::credentials_t credentials;
//         authentication::serverID_t serverID;
//     };
// 
//     struct msg_unregister_request_t {
//         authentication::credentials_t credentials;
//         authentication::serverID_t serverID;
//     };

}

#endif
