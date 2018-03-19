#include "src/connector/data_transfer/context.h"

connector::data_transfer::context::context(session::controller& sessionCtrl, connector::tcp_receiver& tcpRecv, region::controller& regCtrl)
    : m_regCtrl(regCtrl),
      tcpTicketLink(&complete_tcp_ticket_link, &sessionCtrl),
      layerLink(&complete_layer_link, &m_regCtrl),
      dynObjLink(&complete_dynamic_objects_link, &m_regCtrl) {

    tcpRecv.register_callbacks(&tcpTicketLink);
    tcpRecv.register_callbacks(&layerLink);
    tcpRecv.register_callbacks(&dynObjLink);
}

void connector::data_transfer::context::complete_tcp_ticket_link(obj_link<types::data_transfer::content::auth_ticket, session::controller>& obj, session::controller* sessionCtrl) {
    assert(obj.data.size() > 0);
    sessionCtrl->set_tcp_link(obj.data[0]);
}

void connector::data_transfer::context::complete_layer_link(obj_link<types::data_transfer::content::region_layer, region::controller>& obj, region::controller* regCtrl) {
    util::locked_ref<region::context> currentRegion = (*regCtrl)[0];
    currentRegion->load_layers(std::move(obj.data));
    obj.reset();
}

void connector::data_transfer::context::complete_dynamic_objects_link(obj_link<types::data_transfer::content::dynamic_object, region::controller>& obj, region::controller* regCtrl) {
    util::locked_ref<region::context> currentRegion = (*regCtrl)[0];
    currentRegion->load_dynamic_objects(obj.data);
    obj.data.clear();
    obj.reset();
}

