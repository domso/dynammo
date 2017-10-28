#ifndef gameServer_controller_game_user_controller_context_h
#define gameServer_controller_game_user_controller_context_h

#include <unordered_map>
#include "src/controller/game/accept_controller.h"
#include "src/controller/game/map_controller.h"
#include "src/encryption/rsa.h"

namespace controller {
    namespace game {
        class user_controller_context {
        public:
            user_controller_context(accept_controller& acceptThread, map_controller& mapController, encryption::public_key& publicKey);
            
            bool login(const authentication::ticket_t ticket, const authentication::session_t& session, const encryption::signature& signature);
            bool logout(const authentication::session_t& session);
        private:
            struct user_data_t {
                std::shared_ptr<network::tcp_connection<network::ipv4_addr>> connection;
                authentication::session_t session;
            };
            
            std::unordered_map<authentication::sessionID_t, user_data_t> m_userData;
            accept_controller& m_acceptThread;
            map_controller& m_mapController;
            encryption::public_key& m_publicKey;
        };
    }
}

#endif

