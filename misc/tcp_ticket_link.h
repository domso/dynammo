#ifndef gameClient_data_tcp_ticket_link_h
#define gameClient_data_tcp_ticket_link_h

#include <stdint.h>
#include "src/connector/controller.h"

namespace data {
    class tcp_ticket_link {
    public:        
        constexpr static const uint8_t id = 0;
        
        static connector::tcp_receiver::target configure(data::tcp_ticket_link* obj, connector::controller* ctrl);        
        static bool complete(data::tcp_ticket_link* obj, connector::controller* ctrl);
        
        uint32_t value;
    };
}

#endif
