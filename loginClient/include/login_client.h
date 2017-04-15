#ifndef loginClient_login_client_h
#define loginClient_login_client_h

#include <thread>
#include "include/data/context.h"
#include "include/message/msg_controller.h"

//______________________________________________________________________________________________________
//
// client logic for login
//______________________________________________________________________________________________________
class login_client {
public:
    //______________________________________________________________________________________________________
    //
    // Configuration for the login-structure
    //______________________________________________________________________________________________________
    struct config_t {
        uint16_t port;
        int msg_buffer_size;
    };
    //______________________________________________________________________________________________________
    //
    // Description:
    // - creates the internal structures with the given configuration
    // Parameter:
    // -config: reference to configuration
    //______________________________________________________________________________________________________
    login_client(const config_t& config);

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
    // - serverID: id of destination-server
    // - publicKey: pointer to valid instance or nullptr if no encryption is needed
    //______________________________________________________________________________________________________
    void setServer(const network::ipv4_addr& addr, const authentication::serverID_t serverID, encryption::public_key* publicKey = nullptr);
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
    // - requests login on the current destination-server
    // Parameter:
    // - credentials: user-information
    // Return:
    // - true  | on success
    // - false | on error
    //______________________________________________________________________________________________________
    bool requestLogin(const authentication::credentials_t credentials);
    //______________________________________________________________________________________________________
    //
    // Description:
    // - waits for the requested login-ticket (requestLogin()) and stores it into 'ticket'
    // Parameters:
    // - ticket: reference to the output-instance
    // - timeout: maximal wait-time
    // Return:
    // - true  | on success
    // - false | on timeout
    //______________________________________________________________________________________________________
    bool waitForLogin(authentication::ticket_t& ticket, double timeout);
    //______________________________________________________________________________________________________
    //
    // Description:
    // - requests logout on the current destination-server
    // Return:
    // - true  | on success
    // - false | on error
    //______________________________________________________________________________________________________
    bool requestLogout();
    //______________________________________________________________________________________________________
    //
    // Description:
    // - waits for the requested logout (requestLogout())
    // Parameters:
    // - timeout: maximal wait-time
    // Return:
    // - true  | on success
    // - false | on timeout
    //______________________________________________________________________________________________________
    bool waitForLogout(double timeout);


private:
    static void execute(login_client* client);

    config_t config_;

    network::ipv4_addr addr_;
    encryption::public_key* publicKey_;

    std::thread main_thread_;
    data::context context_;
    message::msg_controller<data::context> controller_;
    network::pkt_buffer uncryptedOutputBuffer_;
    network::pkt_buffer encryptedOutputBuffer_;
};

#endif
