#ifndef gameClient_data_context_h
#define gameClient_data_context_h

#include "src/connector/controller.h"
#include "src/data/tcp_ticket_link.h"
#include "src/data/region_data_link.h"

namespace data {
    class context {
    public:
        context(connector::controller& ctrl);
        
        tcp_ticket_link tcpTicket;
        region_data_link regionData;
    };
}

#endif
