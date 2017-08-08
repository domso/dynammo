#ifndef gameServer_controller_game_user_controller_h
#define gameServer_controller_game_user_controller_h

#include <thread>
#include <stdint.h>

#include "src/authentication/types.h"
#include "src/message/msg_controller.h"
#include "src/controller/game/accept_controller.h"
#include "src/controller/game/user_controller_context.h"

namespace controller {
    namespace game {
        class user_controller {
        public:
            user_controller(const int16_t port, accept_controller& acceptThread);

            ~user_controller();


        private:




            user_controller_context m_currentContext;
            message::msg_controller<user_controller_context> m_controller;
            std::thread m_controllerThread;
        };
    }
}

#endif
