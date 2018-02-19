#include "src/connector/worker.h"
#include "src/message/content.h"
#include "src/connector/message/info.h"
#include "src/connector/message/auth.h"

connector::worker::worker(connector::context& context) : m_msgCtrl(bufferSize) {    
    m_msgCtrl.init(1851);
    m_msgCtrl.register_handler<message::callback::info, connector::context>(&context);
    m_msgCtrl.register_handler<message::callback::auth, connector::context>(&context);
    m_thread = std::thread(&message::msg_controller::recv, &m_msgCtrl);
}

connector::worker::~worker() {
    m_msgCtrl.close();

    if (m_thread.joinable()) {
        m_thread.join();
    }
}
