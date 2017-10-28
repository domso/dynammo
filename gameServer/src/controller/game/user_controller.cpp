#include "src/controller/game/user_controller.h"
#include "src/message/game/msg_login.h"
#include "src/message/game/msg_logout.h"

namespace controller {
    namespace game {
        user_controller::user_controller(accept_controller& acceptThread, map_controller& mapController, encryption::public_key& publicKey) : m_currentContext(acceptThread, mapController, publicKey), m_controller(1000) {

        }
        
        bool user_controller::init(const int16_t port) {
            bool result = true;
            result &= m_controller.registerHandler<message::game::msg_login>();
            result &= m_controller.registerHandler<message::game::msg_logout>();
            
            result &= m_controller.init(port, &m_currentContext);
            
            if (!result) {
                return false;
            }

            m_controllerThread = std::thread(&message::msg_controller<user_controller_context>::recv, &m_controller, nullptr, nullptr);

            return true;
        }

        user_controller::~user_controller() {
            m_controller.close();

            if (m_controllerThread.joinable()) {
                m_controllerThread.join();
            }
        }
    }
}
