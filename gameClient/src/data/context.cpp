#include "src/data/context.h"

data::context::context(connector::controller& ctrl) {
    ctrl.register_data_obj<data::tcp_ticket_link>(tcpTicket);
    ctrl.register_data_obj<data::region_data_link>(regionData);
}
