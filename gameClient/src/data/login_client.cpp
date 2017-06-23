#include "include/data/login_client.h"

#include <thread>
#include "include/data/login_context.h"
#include "include/message/msg_controller.h"
#include "include/message/msg_info.h"
#include "include/message/msg_auth.h"

namespace data {
    login_client::login_client(const uint16_t port, const int msgBufferSize) : localPort_(port), publicKey_(nullptr), controller_(msgBufferSize), encryptedOutputBuffer_(msgBufferSize), uncryptedOutputBuffer_(msgBufferSize) {

    }

    login_client::~login_client() {
        if (mainThread_.joinable()) {
            mainThread_.join();
        }
    }

    bool login_client::init() {
        bool result = true;
        result &= controller_.registerHandler<message::msg_info>();
        result &= controller_.registerHandler<message::msg_auth>();

        result &= controller_.init(localPort_, &context_);

        if (!result) {
            return false;
        }

        mainThread_ = std::thread(&message::msg_controller<data::login_context>::recv, &controller_, nullptr, publicKey_);
        return true;
    }

    void login_client::close() {
        controller_.close();
    }

    void login_client::setServer(const network::ipv4_addr& addr, encryption::public_key* publicKey) {
        addr_ = addr;
        publicKey_ = publicKey;
    }

    bool login_client::requestInfo() {
        return controller_.execRequest<message::msg_info>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }

    bool login_client::waitForInfo(authentication::server_info_t& info, double timeout) {
        return context_.waitForServerInfo(info, timeout);
    }

    bool login_client::requestSession(const authentication::credentials_t& credentials) {
        context_.setCredentials(credentials);
        return controller_.execRequest<message::msg_auth>(addr_, uncryptedOutputBuffer_, encryptedOutputBuffer_, context_, nullptr, publicKey_);
    }

    bool login_client::waitForSession(authentication::session_t& session, double timeout) {
        return context_.waitForSession(session, timeout);
    }
}
