#ifndef gameServer_controller_account_session_login_server_h
#define gameServer_controller_account_session_login_server_h

#include <thread>
#include "src/controller/account/login_controller_context.h"
#include "src/message/msg_controller.h"

namespace controller {
    namespace account {
        //______________________________________________________________________________________________________
        //
        // gameServer logic for login
        //______________________________________________________________________________________________________
        class login_controller {
        public:
            //______________________________________________________________________________________________________
            //
            // Description:
            // - creates the internal structures with the given configuration
            // Parameter:
            // -port: udp-socket-port for communication
            // -msgBufferSize: maximal size of internal buffers
            //______________________________________________________________________________________________________
            login_controller(const uint16_t port, const int msgBufferSize);

            ~login_controller();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - initializes the internal structures
            // - starts a new thread for session_login_server::execute()
            // Return:
            // - true  | on success
            // - false | on error
            //______________________________________________________________________________________________________
            bool init();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - tries to close the login-structure
            // - does not wait/block!
            //______________________________________________________________________________________________________
            void close();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - sets server as new destination
            // Parameters:
            // - addr: new destination address
            // - publicKey: pointer to valid instance or nullptr if no encryption is needed
            //______________________________________________________________________________________________________
            void setServer(const network::ipv4_addr& addr, encryption::public_key* publicKey = nullptr);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - requests information about the current destination-server
            // Return:
            // - true  | on success
            // - false | on error
            //______________________________________________________________________________________________________
            bool requestInfo();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - waits for the requested info (requestInfo()) and stores them into 'info'
            // Parameters:
            // - info: reference to the output-instance
            // - timeout: maximal wait-time
            // Return:
            // - true  | on success
            // - false | on timeout
            //______________________________________________________________________________________________________
            bool waitForInfo(authentication::server_info_t& info, double timeout);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - requests registration on the current destination-server
            // Parameters:
            // - serverID: ID of service/server
            // - credentials: userdata for the destination-server
            // Return:
            // - true  | on success
            // - false | on error
            //______________________________________________________________________________________________________
            bool registerService(const authentication::serverID_t serverID, const authentication::credentials_t credentials);
            //______________________________________________________________________________________________________
            //
            // Description:
            // - requests unregistration on the current destination-server
            // Return:
            // - true  | on success
            // - false | on error
            //______________________________________________________________________________________________________
            bool unregisterService();
            //______________________________________________________________________________________________________
            //
            // Description:
            // - gets the current registration-status
            // Parameters:
            // - state: reference to the output-instance
            // - timeout: maximal wait-time
            // Return:
            // - true  | on success
            // - false | on error
            //______________________________________________________________________________________________________
            bool waitForRegisterState(bool& state, double timeout);
        private:
            uint16_t m_localPort;
            network::ipv4_addr m_addr;
            encryption::public_key* m_publicKey;

            std::thread m_mainThread;
            controller::account::login_controller_context m_context;
            message::msg_controller<controller::account::login_controller_context> m_controller;
            network::pkt_buffer m_uncryptedOutputBuffer;
            network::pkt_buffer m_encryptedOutputBuffer;
        };
    }
}

#endif

