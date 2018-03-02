#ifndef gameClient_data_context_h
#define gameClient_data_context_h

#include "src/connector/controller.h"
#include "src/data/obj_link.h"

#include "src/message/msg_controller.h"
#include "src/connector/tcp_receiver.h"
#include "src/message/auth.h"

#include "src/region/dynamic_obj.h"
#include "src/region/context.h"
#include "src/region/controller.h"
#include <iostream>

namespace data {
    class context {
    public:
        context(connector::controller& connCtrl, region::controller& regCtrl);

        region::controller& m_regCtrl;
        
        obj_link<0, uint32_t, connector::controller> tcpTicketLink;
        obj_link<1, region::layer<uint32_t>, region::controller> layerLink;
        obj_link<2, region::dynamic_obj, region::controller> dynObjLink;
    private:
        static void complete_tcp_ticket_link(obj_link<0, uint32_t, connector::controller>& obj, connector::controller* ctrl);        
        static void complete_layer_link(obj_link<1, region::layer<uint32_t>, region::controller>& obj, region::controller* regCtrl);        
        static void complete_dynamic_objects_link(obj_link<2, region::dynamic_obj, region::controller>& obj, region::controller* regCtrl);
        
        
    };
}

#endif
