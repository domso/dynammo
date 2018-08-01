#include "src/connector/msg_transfer/region_action.h"

bool connector::msg_transfer::region_action::request(
    message::msg_header_t& header,
    network::ipv4_addr& destAddr,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    void*, connector::context* context
) {
    return false;
}

message::msg_status_t connector::msg_transfer::region_action::requestHandler(
    message::msg_header_t& header,
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    auto request = inputBuffer.get_next<content::types::request>();
    auto response = outputBuffer.push_next<content::types::response>();
    auto result = message::status::error::unknown;

    if (request != nullptr && response != nullptr) {
        response->sessionID = request->sessionID;
        response->regionID = request->regionID;
        response->actionID = request->actionID;
        result = verify_request(request, header, srcAddr, inputBuffer, outputBuffer, socket, options, context);
    }

    header.status = result;
    return result;
}

message::msg_status_t connector::msg_transfer::region_action::responseHandler(
    message::msg_header_t& header,
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    return message::status::error::unknown;
}

void connector::msg_transfer::region_action::udp_broadcast(
    const network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    const std::unordered_set<uint32_t>& users,
    session::controller& sessionCtrl
) {
    for (uint32_t id : users) {
        auto user = sessionCtrl.get_user(id);

        if (user) {
            socket.send_pkt((*user)->get_addr(), outputBuffer, 0);
        }
    }
}

void connector::msg_transfer::region_action::tcp_broadcast(
    region::context& region,
    const std::vector<region::static_obj>& statics,
    const std::vector<region::dynamic_obj>& dynamics,
    const std::vector<region::layer<uint32_t>> layers,
    const std::unordered_set<uint32_t>& users,
    session::controller& sessionCtrl
) {
    for (uint32_t id : users) {
        auto user = sessionCtrl.get_user(id);

        if (user) {            
            if (!layers.empty()) {
                (*user)->send<::types::data_transfer::content::region_layer>(layers);
            }
            
            if (!statics.empty()) {
                (*user)->send<::types::data_transfer::content::static_object>(statics);
            }

            if (!dynamics.empty()) {
                (*user)->send<::types::data_transfer::content::dynamic_object>(dynamics);
            }
        } else {
            region.remove_user(id);
        }
    }
}

void connector::msg_transfer::region_action::send_tcp(
    region::context& region,
    session::controller& sessionCtrl
) {
    std::vector<region::static_obj> statics;
    std::vector<region::dynamic_obj> dynamics;
    std::vector<region::layer<uint32_t>> layers;
    
    for (auto& o : region.changed_static_objects()) {
        statics.push_back(region.all_static_objects().at(o));
    }

    if (region.changed_dynamic_objects().size() > 4) {
        for (auto& o : region.changed_dynamic_objects()) {
            dynamics.push_back(region.all_dynamic_objects().at(o));
        }
    }
    
    for (auto& o : region.changed_layers()) {
        layers.push_back(region.all_layers().at(o));
    }

    tcp_broadcast(region, statics, dynamics, layers, region.affected_users(), sessionCtrl);
}

void connector::msg_transfer::region_action::send_udp(
    region::context& region,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    session::controller& sessionCtrl
) {
    if (region.changed_dynamic_objects().size() <= 4) {
        for (auto& o : region.changed_dynamic_objects()) {
            region::dynamic_obj* dest = outputBuffer.push_next<region::dynamic_obj>();

            if (dest != nullptr) {
                *dest = region.all_dynamic_objects().at(o);
            }
        }
    }

    udp_broadcast(outputBuffer, socket, region.affected_users(), sessionCtrl);
}

message::msg_status_t connector::msg_transfer::region_action::verify_request(
    content::types::request* request,
    message::msg_header_t& header,
    network::ipv4_addr& srcAddr,
    network::pkt_buffer& inputBuffer,
    network::pkt_buffer& outputBuffer,
    network::udp_socket<network::ipv4_addr>& socket,
    message::msg_option_t& options,
    connector::context* context
) {
    auto user = context->sessionCtrl.get_user(request->sessionID);

    if (user) {
        if ((*user)->verify_buffer(inputBuffer)) {
            auto region = context->regionCtrl.get_region(request->regionID);

            if (region->action((*user)->get_accountID(), request->sessionID, request->actionID)) {
                header.status = message::status::ok;
                (*user).release();
                send_udp(*region.data(), outputBuffer, socket, context->sessionCtrl);
                send_tcp(*region.data(), context->sessionCtrl);
                region->commit();
                
                return message::status::close;
            } else {
                return message::status::error::execution;
            }
        } else {
            return message::status::error::access_denied;
        }
    } else {
        return message::status::error::unavailable;
    }
}


