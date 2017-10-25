#include "src/controller/game/user_controller_context.h"

namespace controller {
    namespace game {
        user_controller_context::user_controller_context(accept_controller& acceptThread) : acceptThread(acceptThread) {

        }

        bool user_controller_context::login(const authentication::ticket_t ticket, const authentication::session_t& session) {
            user_data_t user;
            user.connection = acceptThread.get_connection(ticket);
            
            if (user.connection != nullptr) {
                user.session = session;
                userData[session.accountID] = user;
                
                return true;
            }
            
            return false;
        }
        
        bool user_controller_context::logout(const authentication::session_t& session) {
            auto user = userData.find(session.accountID);
            
            if (user != userData.end()) {
//                 if (session == user->second.session) {
//                     userData.erase(user);
//                     
//                     return true;
//                 }
            }
            
            return false;
        }
    }
}

