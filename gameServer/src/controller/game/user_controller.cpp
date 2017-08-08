#include "src/controller/game/user_controller.h"

namespace controller {
    namespace game {
        user_controller::user_controller(const int16_t port, accept_controller& acceptThread) : m_currentContext(acceptThread), m_controller(1000) {
            m_controller.init(port, &m_currentContext);

            m_controllerThread = std::thread(&message::msg_controller<user_controller_context>::recv, &m_controller, nullptr, nullptr);

        }

        user_controller::~user_controller() {
            m_controller.close();

            if (m_controllerThread.joinable()) {
                m_controllerThread.join();
            }
        }
    }
}
