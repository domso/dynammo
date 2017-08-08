#ifndef gameServer_controller_game_user_controller_context_h
#define gameServer_controller_game_user_controller_context_h

#include <unordered_map>
#include "src/controller/game/accept_controller.h"

namespace controller {
    namespace game {
        class user_controller_context {
        public:
            user_controller_context(accept_controller& acceptThread);
            
            bool login(const authentication::ticket_t ticket, const authentication::session_t& session);
            bool logout(const authentication::session_t& session);
        private:
            struct user_data_t {
                std::shared_ptr<network::tcp_connection<network::ipv4_addr>> connection;
                authentication::session_t session;
            };
            
            std::unordered_map<authentication::sessionID_t, user_data_t> userData;
            accept_controller& acceptThread;
        };
    }
}

#endif

