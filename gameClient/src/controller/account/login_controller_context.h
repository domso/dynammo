#ifndef gameClient_controller_account_login_controller_context_h
#define gameClient_controller_account_login_controller_context_h

#include <vector>
#include "src/authentication/types.h"
#include "src/encryption/rsa.h"
#include "src/util/wait_lock.h"

namespace controller {
    //______________________________________________________________________________________________________
    //
    // login-layer for gameClient
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
        // - gets current session and stores it into 'session'
        // Parameter:
        // - session: reference to an output instance
        //______________________________________________________________________________________________________
        void getSession(authentication::signed_session_t& session);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - stores 'session' as new session of the current user
        // Parameter:
        // - info: reference to an input instance
        // - sigLen: length of signature
        // - signature: pointer to the session-signature
        //______________________________________________________________________________________________________
        void setSession(const authentication::session_t& session, const int sigLen, const unsigned char* signature);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets current session and stores it into 'session'
        // - waits until the data is marked as an update
        // Parameter:
        // - session: reference to an output instance
        // - timeOut: maximal number of seconds to wait for an update
        // Return:
        // - true  | on success
        // - false | on timeout
        //______________________________________________________________________________________________________
        bool waitForSession(authentication::signed_session_t& session, double timeOut);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets current loginState and stores it into 'loginState'
        // Parameter:
        // - loginState: reference to an output instance
        //______________________________________________________________________________________________________
        void getLoginState(bool& loginState);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - stores 'loginState' as new loginState of the current user
        // Parameter:
        // - info: reference to an input instance
        //______________________________________________________________________________________________________
        void setLoginState(const bool& loginState);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - gets current loginState and stores it into 'loginState'
        // - waits until the data is marked as an update
        // Parameter:
        // - loginState: reference to an output instance
        // - timeOut: maximal number of seconds to wait for an update
        // Return:
        // - true  | on success
        // - false | on timeout
        //______________________________________________________________________________________________________
        bool waitForLoginState(bool& loginState, double timeOut);
    private:
        encryption::public_key m_key;
        util::wait_lock::context_t<authentication::server_info_t> m_serverInfo;
        util::wait_lock::context_t<authentication::signed_session_t> m_signedSession;
        util::wait_lock::context_t<authentication::credentials_t> m_credentials;
        util::wait_lock::context_t<bool> m_loginState;

    };
}

#endif





