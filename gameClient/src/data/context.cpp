#include "src/data/context.h"

data::context::context(connector::controller& connCtrl, region::controller& regCtrl)
    : m_regCtrl(regCtrl),
      tcpTicketLink(&complete_tcp_ticket_link, &connCtrl),
      layerLink(&complete_layer_link, &m_regCtrl),
      dynObjLink(&complete_dynamic_objects_link, &m_regCtrl) {
    connCtrl.register_data_obj<obj_link<0, uint32_t, connector::controller>>(tcpTicketLink);
    connCtrl.register_data_obj<obj_link<1, region::layer<uint32_t>, region::controller>>(layerLink);
    connCtrl.register_data_obj<obj_link<2, region::dynamic_obj, region::controller>>(dynObjLink);
}

void data::context::complete_tcp_ticket_link(obj_link<0, uint32_t, connector::controller>& obj, connector::controller* ctrl) {
    assert(obj.data.size() > 0);
    ctrl->get_message_obj<message::auth>().request.tcpTicket = obj.data[0];
    ctrl->exec_request<message::auth>();
}

void data::context::complete_layer_link(obj_link<1, region::layer<uint32_t>, region::controller>& obj, region::controller* regCtrl) {
    util::locked_ref<region::context> currentRegion = (*regCtrl)[0];
    currentRegion->load_layers(std::move(obj.data));
}

void data::context::complete_dynamic_objects_link(obj_link<2, region::dynamic_obj, region::controller>& obj, region::controller* regCtrl) {
    util::locked_ref<region::context> currentRegion = (*regCtrl)[0];
    currentRegion->load_dynamic_objects(obj.data);
    obj.data.clear();
}

