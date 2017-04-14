#ifndef loginClient_login_client_h
#define loginClient_login_client_h

#include <thread>
#include "include/data/context.h"
#include "include/message/msg_controller.h"
#include "include/message/msg_info.h"
#include "include/message/msg_login.h"
#include "include/message/msg_logout.h"
#include "include/message/msg_lookup.h"

class login_client {
public:

    struct config_t {
        uint16_t port;
        int msg_buffer_size;
    };

    login_client() : publicKey_(nullptr), controller_(1000), encryptedOutputBuffer_(1000), uncryptedOutputBuffer_(1000) {

    }

    ~login_client() {
        if (main_thread_.joinable()) {
            main_thread_.join();
        }
    }

    bool init(const config_t& config) {
        controller_.registerHandler<message::msg_info>();
        controller_.registerHandler<message::msg_login>();
        controller_.registerHandler<message::msg_logout>();
        controller_.registerHandler<message::msg_lookup>();

        if (!controller_.init(config.port, &context_)) {
            return false;
        }

        main_thread_ = std::thread(execute, this);
    }

    static void execute(login_client* client) {
        if (client != nullptr) {
            client->controller_.recv(nullptr, client->publicKey_);
        }
    }

    void close() {
        controller_.close();
    }

    void setServer(const network::ipv4_addr& addr, encryption::public_key* publicKey = nullptr) {
        addr_ = addr;
        publicKey_ = publicKey;
    }

    bool requestInfo() {
        return controller_.execRequest<message::msg_info>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }

    bool waitForInfo(authentication::server_info_t& info, double timeout) {
        return context_.waitForServerInfo(info, timeout);
    }

    bool login(const authentication::credentials_t credentials) {
        context_.setCredentials(credentials);
        return controller_.execRequest<message::msg_login>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }

    bool waitForLogin(authentication::ticket_t& ticket, double timeout) {
        return context_.waitForTicket(ticket, timeout);
    }

    bool logout() {
        return controller_.execRequest<message::msg_logout>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }

    bool waitForLogout(double timeout) {
        return context_.waitForClearedTicket(timeout);
    }


private:
    network::ipv4_addr addr_;
    encryption::public_key* publicKey_;

    std::thread main_thread_;
    data::context context_;
    message::msg_controller<data::context> controller_;
    network::pkt_buffer uncryptedOutputBuffer_;
    network::pkt_buffer encryptedOutputBuffer_;
};

#endif
