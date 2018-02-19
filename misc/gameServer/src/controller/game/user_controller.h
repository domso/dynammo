#ifndef gameServer_controller_game_user_controller_h
#define gameServer_controller_game_user_controller_h

#include <thread>
#include <stdint.h>

#include "src/authentication/types.h"
#include "src/message/msg_controller.h"
#include "src/controller/game/accept_controller.h"
#include "src/controller/game/map_controller.h"
#include "src/controller/game/user_controller_context.h"
#include "src/encryption/rsa.h"

namespace controller {
    namespace game {
        class user_controller {
        public:
            user_controller(accept_controller& acceptThread, map_controller& mapController, encryption::public_key& publicKey);
            ~user_controller();

            bool init(const int16_t port);

        private:
            user_controller_context m_currentContext;
            message::msg_controller<user_controller_context> m_controller;
            std::thread m_controllerThread;
        };
    }
}

#endif
