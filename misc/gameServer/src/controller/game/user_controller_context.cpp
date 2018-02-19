#include "src/controller/game/user_controller_context.h"
#include "src/util/mem.h"

namespace controller {
    namespace game {
        user_controller_context::user_controller_context(accept_controller& acceptThread, map_controller& mapController, encryption::public_key& publicKey) : m_acceptThread(acceptThread), m_mapController(mapController), m_publicKey(publicKey) {

        }

        bool user_controller_context::login(const authentication::ticket_t ticket, const authentication::session_t& session, const encryption::signature& signature) {
            bool result = false;
            user_data_t user;

            if (encryption::verify<authentication::session_t>(m_publicKey, signature, &session)) {
                user.connection = m_acceptThread.get_connection(ticket);

                if (user.connection != nullptr) {
                    user.session = session;
                    m_userData[session.accountID] = user;

                    result = true;
                }
            }

            return result;
        }

        bool user_controller_context::logout(const authentication::session_t& session) {
            bool result = false;
            auto user = m_userData.find(session.accountID);

            if (user != m_userData.end()) {
                if (util::mem::cmp<authentication::session_t>(&session, &user->second.session)) {
                    m_userData.erase(user);

                    result = true;
                }
            }

            return result;
        }
    }
}

