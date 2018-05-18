#pragma once

#include <mutex>

#include "src/util/event_controller.h"
#include "src/util/config_file.h"
#include "src/authentication/types.h"
#include "src/encryption/rsa.h"
#include "src/types/game_events.h"

namespace session {
    class controller {
    public:
        controller(util::event_controller<types::game_events>& eventCtrl, util::config_file& config);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        
        int get_signature_length();        
        bool sign_data(encryption::signature& destSignature, const int8_t* data, const int length);
        
        void set_tcp_link(const authentication::ticket_t newLink);
        authentication::ticket_t get_tcp_link();
        
        authentication::accountID_t get_accountID();   
        
        void set_auth_state(const bool state);
        bool get_auth_state();


        uint32_t regUserID = 0; //TODO
    private:
        void load_from_config();
        
        std::mutex m_mutex;
        util::event_controller<types::game_events>& m_eventCtrl;
        
        struct session_info {
            bool valid = false;
            bool auth_state = false;
            authentication::accountID_t accountID;        
            authentication::ticket_t tcpTicket;
            
            encryption::private_key privateKey;
        };        
        
        util::config_file& m_config;
        session_info m_currentSession;
    };
}
