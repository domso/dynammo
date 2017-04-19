#ifndef general_auth_types_h
#define general_auth_types_h

#include "network/ip_addr.h"

namespace authentication {

    typedef int32_t accountID_t;
    typedef int32_t serverID_t;
    typedef int32_t sessionID_t;
    typedef int32_t permissions_t;
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
    };
    //______________________________________________________________________________________________________
    struct ticket_t {
        accountID_t accountID;
        serverID_t serverID;
        int32_t issuer;
        int32_t loginTime;
        permissions_t permissions;
        random_key_t key;
        network::ipv4_addr userAddr;
    };
    //______________________________________________________________________________________________________
    //
    // the gameServer stores the current ticket with the session
    //______________________________________________________________________________________________________
    struct user_data_t {
        session_t session;
        ticket_t ticket;
    };

}

#endif
