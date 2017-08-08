#ifndef gameServer_controller_account_login_controller_context_h
#define gameServer_controller_account_login_controller_context_h

#include <queue>
#include "src/authentication/types.h"
#include "src/encryption/rsa.h"
#include "src/util/wait_lock.h"

namespace controller {
    namespace account {
        //______________________________________________________________________________________________________
        //
        // session-layer for gameServer
        //______________________________________________________________________________________________________
        class login_controller_context {
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
            // - credentials: reference to an output instance
            //______________________________________________________________________________________________________
            void getCredentials(authentication::credentials_t& credentials);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - stores 'credentials' as the new credentials of the current user
            // Parameter:
            // - credentials: reference to an input instance
            //______________________________________________________________________________________________________
            void setCredentials(const authentication::credentials_t& credentials);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - gets the serverID of the current service and stores them in 'serverID'
            // Parameter:
            // - credentials: reference to an output instance
            //______________________________________________________________________________________________________
            void getServerID(authentication::serverID_t& serverID);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - stores 'serverID' as the new ID of the current service/server
            // Parameter:
            // - credentials: reference to an input instance
            //______________________________________________________________________________________________________
            void setServerID(const authentication::serverID_t& serverID);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - stores 'state' as new registration-status
            // Parameter:
            // - info: reference to an input instance
            //______________________________________________________________________________________________________
            void setRegisterState(const bool state);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - gets the registration-status and stores it in 'state'
            // - waits until the data is marked as an update
            // Parameter:
            // - info: reference to an output instance
            // - timeOut: maximal number of seconds to wait for an update
            // Return:
            // - true  | on success
            // - false | on timeout
            //______________________________________________________________________________________________________
            bool waitForRegisterState(bool& state, double timeOut);
        private:
            encryption::public_key m_key;
            util::wait_lock::context_t<authentication::server_info_t> m_serverInfo;
            util::wait_lock::context_t<authentication::credentials_t> m_credentials;

            util::wait_lock::context_t<authentication::serverID_t> m_serverID;
            util::wait_lock::context_t<bool> m_registerState;
        };
    }
}

#endif





