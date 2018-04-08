#pragma once

#include <mutex>

#include "src/authentication/types.h"
#include "src/encryption/rsa.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"

namespace session {
    class controller {
    public:
        controller(util::event_controller<types::game_events>& eventCtrl);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        
        void load_local_account(const authentication::accountID_t accID);
        void close_local_account(const authentication::accountID_t accID);
        int get_signature_length();        
        bool sign_data(encryption::signature& destSignature, const int8_t* data, const int length);
        
        void set_tcp_link(const authentication::ticket_t newLink);
        authentication::ticket_t get_tcp_link();
    private:
        std::mutex m_mutex;
        util::event_controller<types::game_events>& m_eventCtrl;
        
        struct session_info {
            bool valid = false;
            authentication::accountID_t accountID;        
            authentication::ticket_t tcpTicket;
            
            encryption::private_key privateKey;
        };        
        
        session_info m_currentSession;
    };
}
