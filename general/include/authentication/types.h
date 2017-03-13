#ifndef general_auth_types_h
#define general_auth_types_h

#include "network/ip_addr.h"

namespace authentication {
    
    typedef int32_t accountID_t;
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
    // the identification after login on the loginServer the accountID and credentials are neccessary
    //______________________________________________________________________________________________________
    struct identification_t {
        accountID_t accountID;
        credentials_t credentials;
    };
    //______________________________________________________________________________________________________
    //
    // basic information about a server
    //______________________________________________________________________________________________________
    struct server_info_t {
        int32_t id;
        int32_t numClients;
        int32_t uptime;
    };
    //______________________________________________________________________________________________________
    //
    // any authentication on a non-loginServer is done by this ticket
    // the server can then validate this ticket by the loginServer
    //______________________________________________________________________________________________________
    struct ticket_t {
        accountID_t accountID;
        int32_t issuer;
        int32_t loginTime;
        permissions_t permissions;
        random_key_t key;
        network::ipv4_addr userAddr;
    };
    //______________________________________________________________________________________________________
    //
    // the loginServer stores the current ticket with the credentials
    //______________________________________________________________________________________________________
    struct user_data_t {
        ticket_t ticket;
        credentials_t credentials;
    };
    
}

#endif
