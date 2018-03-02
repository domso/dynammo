#include "src/data/tcp_ticket_link.h"
#include "src/message/auth.h"

connector::tcp_receiver::target data::tcp_ticket_link::configure(data::tcp_ticket_link* obj, connector::controller* ctrl) {
    connector::tcp_receiver::target result;
    result.set<uint32_t>(&obj->value);
    return result;
}

bool data::tcp_ticket_link::complete(data::tcp_ticket_link* obj, connector::controller* ctrl) {
    ctrl->get_message_obj<message::auth>().request.tcpTicket = obj->value;
    ctrl->exec_request<message::auth>();
    return true;
}
