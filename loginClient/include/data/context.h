#ifndef loginClient_data_context_h
#define loginClient_data_context_h

#include <unordered_map>
#include <queue>
#include "include/authentication/types.h"
#include "include/encryption/rsa.h"
#include "include/util/wait_lock.h"

namespace data {
    //______________________________________________________________________________________________________
    //
    // userdata-layer for client
    //______________________________________________________________________________________________________
    class context {
    public:
        //______________________________________________________________________________________________________
        //
        // Return:
        // - reference to internal public key
        //______________________________________________________________________________________________________
        encryption::public_key& getKey();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets information about the current server and stores them in 'info'
        // Parameter:
        // - info: reference to an output instance
        //______________________________________________________________________________________________________
        void getServerInfo(authentication::server_info_t& info);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - stores 'info' as new information about the current server
        // Parameter:
        // - info: reference to an input instance
        //______________________________________________________________________________________________________
        void setServerInfo(const authentication::server_info_t& info);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets information about the current server and stores them in 'info'
        // - waits until the data is marked as an update
        // Parameter:
        // - info: reference to an output instance
        // - timeOut: maximal number of seconds to wait for an update
        // Return:
        // - true  | on success
        // - false | on timeout
        //______________________________________________________________________________________________________
        bool waitForServerInfo(authentication::server_info_t& info, double timeOut);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets the credentials of the current user and stores them in 'credentials'
        // Parameter:
        // - info: reference to an output instance
        //______________________________________________________________________________________________________
        void getCredentials(authentication::credentials_t& credentials);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - stores 'credentials' as the new credentials of the current user
        // Parameter:
        // - info: reference to an input instance
        //______________________________________________________________________________________________________
        void setCredentials(const authentication::credentials_t& credentials);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets the identification of the current user and stores them in 'identification'
        // Parameter:
        // - info: reference to an output instance
        //______________________________________________________________________________________________________
        void getIdentification(authentication::identification_t& identification);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - stores 'identification' as the new identification of the current user
        // Parameter:
        // - info: reference to an input instance
        //______________________________________________________________________________________________________
        void setIdentification(const authentication::identification_t& identification);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - stores 'ticket' as the new ticket of the current user
        // - triggers an update for waitForTicket
        // Parameter:
        // - info: reference to an input instance
        //______________________________________________________________________________________________________
        void setTicket(const authentication::ticket_t& ticket);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets the ticket of the current user and stores it in 'ticket'
        // - waits until the data is marked as an update
        // - checks if the ticket is not empty (accountID != 0)
        // Parameter:
        // - ticket: reference to an output instance
        // - timeOut: maximal number of seconds to wait for an update
        // Return:
        // - true  | on success
        // - false | on timeout
        //______________________________________________________________________________________________________
        bool waitForTicket(authentication::ticket_t& ticket, double timeOut);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - clears the ticket and the identification of the current user
        //______________________________________________________________________________________________________
        void clearTicket();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - waits until the data is marked as an update
        // - checks if the ticket was cleared (accountID == 0)
        // Parameter:
        // - timeOut: maximal number of seconds to wait for an update
        // Return:
        // - true  | on success
        // - false | on timeout
        //______________________________________________________________________________________________________
        bool waitForClearedTicket(double timeOut);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - pushes a new ticket in the validation-queue
        // Parameter:
        // - ticket: reference to an input instance
        //______________________________________________________________________________________________________
        void pushTicket(const authentication::ticket_t& ticket);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - pops and stores the first entry in the validation-queue in 'ticket'
        // Parameter:
        // - ticket: reference to an output instance
        //______________________________________________________________________________________________________
        bool popTicket(authentication::ticket_t& ticket);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - pushes a validated ID into the output-queue
        // Parameter:
        // - accountID: input instance
        //______________________________________________________________________________________________________
        void setValidatedID(const authentication::accountID_t accountID);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets the next validated ID
        // - waits until the data is marked as an update
        // Parameter:
        // - accountID: reference to an output instance
        // - timeOut: maximal number of seconds to wait for an update
        // Return:
        // - true  | on success
        // - false | on timeout
        //______________________________________________________________________________________________________
        bool waitForValidatedID(authentication::accountID_t& accountID, double timeOut);
    private:
        encryption::public_key key_;
        util::wait_lock::context_t<authentication::server_info_t> serverInfo_;
        util::wait_lock::context_t<authentication::identification_t> identification_;
        util::wait_lock::context_t<authentication::ticket_t> ticket_;
        util::wait_lock::context_t<std::queue<authentication::ticket_t>> ticketQueue_;
        util::wait_lock::context_t<std::queue<authentication::accountID_t>> validAccountIDsQueue_;
    };
}

#endif





