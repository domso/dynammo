#pragma once

#include "src/authentication/types.h"
#include "src/connector/tcp_receiver.h"
#include "src/connector/data_transfer/obj_link.h"
#include "src/connector/msg_transfer/auth.h"
#include "src/connector/tcp_receiver.h"
#include "src/region/dynamic_obj.h"
#include "src/region/context.h"
#include "src/region/controller.h"
#include "src/session/controller.h"
#include "src/types/data_transfer/content.h"

namespace connector {
    namespace data_transfer {
        class context {
        public:            
            context(session::controller& sessionCtrl, connector::tcp_receiver& tcpRecv, region::controller& regCtrl);
        private:
            region::controller& m_regCtrl;

            obj_link<types::data_transfer::content::auth_ticket, session::controller> tcpTicketLink;
            obj_link<types::data_transfer::content::region_layer, region::controller> layerLink;
            obj_link<types::data_transfer::content::static_object, region::controller> staticObjLink;
            obj_link<types::data_transfer::content::dynamic_object, region::controller> dynObjLink;
            
            static void complete_tcp_ticket_link(obj_link<types::data_transfer::content::auth_ticket, session::controller>& obj, session::controller* sessionCtrl);
            static void complete_layer_link(obj_link<types::data_transfer::content::region_layer, region::controller>& obj, region::controller* regCtrl);
            static void complete_static_objects_link(obj_link<types::data_transfer::content::static_object, region::controller>& obj, region::controller* regCtrl);
            static void complete_dynamic_objects_link(obj_link<types::data_transfer::content::dynamic_object, region::controller>& obj, region::controller* regCtrl);
        };
    }
}
