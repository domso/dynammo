#ifndef gameClient_datalogin_client_h
#define gameClient_datalogin_client_h

#include <thread>
#include "src/data/login_context.h"
#include "src/message/msg_controller.h"

namespace data {
//______________________________________________________________________________________________________
//
// gameClient logic for login
//______________________________________________________________________________________________________
    class login_client {
    public:
        //______________________________________________________________________________________________________
        //
        // Description:
        // - creates the internal structures with the given configuration
        // Parameter:
        // -port: udp-socket-port for communication
        // -msgBufferSize: maximal size of internal buffers
        //______________________________________________________________________________________________________
        login_client(const uint16_t port, const int msgBufferSize);

        ~login_client();
        //______________________________________________________________________________________________________
        //
        // Description:
        // - initializes the internal structures
        // - starts a new thread for login_client::execute()
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
        // - requests a session from the current destination-server
        // Parameter:
        // - credentials: account-data of the current user
        // Return:
        // - true  | on success
        // - false | on error
        //______________________________________________________________________________________________________
        bool requestSession(const authentication::credentials_t& credentials);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - waits for the requested session (requestSession()) and stores it into 'session'
        // Parameters:
        // - session: reference to the output-instance
        // - timeout: maximal wait-time
        // Return:
        // - true  | on success
        // - false | on timeout
        //______________________________________________________________________________________________________
        bool waitForSession(authentication::session_t& session, double timeout);

    private:

        uint16_t m_localPort;
        network::ipv4_addr m_addr;
        encryption::public_key* m_publicKey;

        std::thread m_mainThread;
        data::login_context m_context;
        message::msg_controller<data::login_context> m_controller;
        network::pkt_buffer m_uncryptedOutputBuffer;
        network::pkt_buffer m_encryptedOutputBuffer;
    };
}

#endif
