#include "include/data/session_login_server.h"

#include <thread>
#include "include/data/session_context.h"
#include "include/message/msg_controller.h"
#include "include/message/msg_info.h"
#include "include/message/msg_auth.h"
#include "include/message/msg_register.h"
#include "include/message/msg_unregister.h"

namespace data {
    session_login_server::session_login_server(const uint16_t port, const int msgBufferSize) : localPort_(port), publicKey_(nullptr), controller_(msgBufferSize), encryptedOutputBuffer_(msgBufferSize), uncryptedOutputBuffer_(msgBufferSize) {

    }

    session_login_server::~session_login_server() {
        if (mainThread_.joinable()) {
            mainThread_.join();
        }
    }

    bool session_login_server::init() {
        bool result = true;
        result &= controller_.registerHandler<message::msg_info>();
        result &= controller_.registerHandler<message::msg_auth>();
        result &= controller_.registerHandler<message::msg_register>();
        result &= controller_.registerHandler<message::msg_unregister>();

        result &= controller_.init(localPort_, &context_);

        if (!result) {
            return false;
        }

        mainThread_ = std::thread(message::msg_controller<data::session_context>::srecv, &(this->controller_), nullptr, publicKey_);
        return true;
    }

    void session_login_server::close() {
        controller_.close();
    }

    void session_login_server::setServer(const network::ipv4_addr& addr, encryption::public_key* publicKey) {
        addr_ = addr;
        publicKey_ = publicKey;
    }

    bool session_login_server::requestInfo() {
        return controller_.execRequest<message::msg_info>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }

    bool session_login_server::waitForInfo(authentication::server_info_t& info, double timeout) {
        return context_.waitForServerInfo(info, timeout);
    }

    bool session_login_server::registerService(const authentication::serverID_t serverID, const authentication::credentials_t credentials) {
        context_.setCredentials(credentials);
        context_.setServerID(serverID);
        return controller_.execRequest<message::msg_register>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }

    bool session_login_server::unregisterService() {
        return controller_.execRequest<message::msg_unregister>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }
    
    bool session_login_server::waitForRegisterState(bool& state, double timeout) {
        return context_.waitForRegisterState(state, timeout);
    }

    
    bool session_login_server::waitForUser(authentication::user_data_t& user, double timeout) {
        return context_.waitForUser(user, timeout);
    }

}
