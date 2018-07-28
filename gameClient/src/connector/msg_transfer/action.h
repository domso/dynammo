#pragma once

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/types/game_events.h"
#include "src/connector/context.h"
#include "src/connector/sign_buffer.h"


namespace connector {
    namespace msg_transfer {
        class action {
        public:
            typedef types::msg_transfer::content::action content;

            constexpr static const auto id = content::id;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, uint64_t eventArg, connector::msg_transfer::data* data) {
                auto request = outputBuffer.push_next<content::types::request>();

//                 if (request != nullptr) {
//                     request->accountID = data->sessionCtrl.get_accountID();
//                     request->regionID = data->sessionCtrl.regUserID;
//                     request->actionID = eventArg;
// 
//                     {
//                         auto region = data->regionCtrl[data->sessionCtrl.regUserID];
//                         request->objID = region->get_selected_character();
//                     }
// 
//                     return sign_buffer(outputBuffer, data->sessionCtrl);
//                 }

                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::data* data) {
                return message::status::close;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::msg_transfer::data* data) {
                auto response = inputBuffer.get_next<content::types::response>();
                auto responseExtension = inputBuffer.get_next<content::types::response_extension>();

                if (header.status == message::status::ok) {
//                     if (response != nullptr) {
//                         auto region = data->regionCtrl[response->regionID];
//                         region->load_dynamic_object(response->obj);
//                     }
// 
//                     if (responseExtension != nullptr) {
//                         {
//                             auto region = data->regionCtrl[responseExtension->oldRegionID];
//                             region->remove_dynamic_object(responseExtension->oldObjID);
//                         }
// 
//                         {
//                             auto region = data->regionCtrl[data->sessionCtrl.regUserID];
//                             if (region->get_selected_character() == responseExtension->oldObjID
//                                     && data->sessionCtrl.regUserID == responseExtension->oldRegionID) {
//                                 data->sessionCtrl.regUserID = response->regionID;
//                             }
//                         }
//                     }
                }

                return message::status::close;
            }
        };
    }
}

