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

void connector::data_transfer::context::complete_tcp_ticket_link(obj_link<0, authentication::ticket_t, session::controller>& obj, session::controller* sessionCtrl) {
    assert(obj.data.size() > 0);
    sessionCtrl->set_tcp_link(obj.data[0]);
}

void connector::data_transfer::context::complete_layer_link(obj_link<1, region::layer<uint32_t>, region::controller>& obj, region::controller* regCtrl) {
    util::locked_ref<region::context> currentRegion = (*regCtrl)[0];
    currentRegion->load_layers(std::move(obj.data));
}

void connector::data_transfer::context::complete_dynamic_objects_link(obj_link<2, region::dynamic_obj, region::controller>& obj, region::controller* regCtrl) {
    util::locked_ref<region::context> currentRegion = (*regCtrl)[0];
    currentRegion->load_dynamic_objects(obj.data);
    obj.data.clear();
}

