#ifndef gameClient_data_region_data_link_h
#define gameClient_data_region_data_link_h

#include <stdint.h>
#include "src/connector/controller.h"
#include "src/util/state_machine.h"
#include "src/message/msg_types.h"
#include "src/region/context.h"

namespace data {
    class region_data_link {
    public:        
        constexpr static const uint8_t id = 1;
        
        static connector::tcp_receiver::target configure(data::region_data_link* obj, connector::controller* ctrl);        
        static bool complete(data::region_data_link* obj, connector::controller* ctrl);
        
        util::state_machine<message::types::states> currentState = message::types::states::ready;
        region::context region0;
    };
}

#endif
