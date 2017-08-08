#ifndef general_auth_types_h
#define general_auth_types_h

#include <vector>
#include "network/ip_addr.h"

namespace authentication {

    typedef int32_t accountID_t;
    typedef int32_t serverID_t;
    typedef int32_t sessionID_t;
    typedef int32_t permissions_t;
    typedef int32_t ticket_t;
    //______________________________________________________________________________________________________
    //
    // random salt
    //______________________________________________________________________________________________________
    struct random_key_t {
        char key[32];
    };
    //______________________________________________________________________________________________________
    //
    // basic credentials containing the username and the random key (see the general authentication concept for more details)
    //______________________________________________________________________________________________________
    struct credentials_t {
        char username[8];
        char key[128];
    };
    //______________________________________________________________________________________________________
    //
    // basic information about a server
    //______________________________________________________________________________________________________
    struct server_info_t {
        int32_t id;
        int32_t uptime;
    };
    //______________________________________________________________________________________________________
    //
    // session identification
    //______________________________________________________________________________________________________
    struct session_t {
        accountID_t accountID;
        sessionID_t sessionID;
        serverID_t serverID;
        int32_t loginTime;
        int32_t issuer;
        permissions_t permissions;
    };
    //______________________________________________________________________________________________________
    //
    // signed session identification
    //______________________________________________________________________________________________________
    struct signed_session_t {
        authentication::session_t session;
        std::vector<unsigned char> signature;
    };
}

#endif
