#pragma once

#include "network/udp_socket.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/connector/context.h"
#include "src/types/game_events.h"
#include "src/connector/verify_buffer.h"

namespace connector {
    namespace msg_transfer {
        class transfer_action {
        public:
            typedef ::types::msg_transfer::content::transfer_action content;
            constexpr static const auto id = content::id;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, void*, connector::context* context) {
                return false;
            }

            static void transfer_to_region(const content::types::request* request, content::types::response* response, util::locked_ref<region::context>& srcRegion, util::locked_ref<region::context>& destRegion, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, connector::context* context) {
                region::dynamic_obj* obj = srcRegion->get_dynamic_obj(request->objID);

                if (obj != nullptr) {
                    destRegion->insert_new_dynamic_object(*obj);
                    srcRegion->remove_dynamic_object(request->objID);
                    response->obj = *obj;

                    for (auto& UserID : srcRegion->get_users()) {
                        if (UserID != request->accountID) {
//                             auto info = context->userCtrl.get_info(UserID);
//                             socket.send_pkt(info->udpAddr, outputBuffer);
                        }
                    }

                    for (auto& UserID : destRegion->get_users()) {
                        if (UserID != request->accountID) {
//                             auto info = context->userCtrl.get_info(UserID);
//                             socket.send_pkt(info->udpAddr, outputBuffer);
                        }
                    }
                }
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                auto request = inputBuffer.get_next<content::types::request>();
                auto response = outputBuffer.push_next<content::types::response>();
                auto result = message::status::error::unknown;
                bool verified = false;

                if (request != nullptr && response != nullptr) {
                    {
//                         auto info = context->userCtrl.get_info(request->accountID);
//                         verified = verify_buffer(inputBuffer, *info.data());
                    }
                    

                    if (verified) {
//                         ::types::game_events action = (::types::game_events) request->actionID;
                        response->srcRegionID = request->srcRegionID;
                        response->destRegionID = request->destRegionID;
                        response->oldObjID = request->objID;

                        if (request->srcRegionID < request->destRegionID) {
                            auto srcRegion = context->regionCtrl.get_region(request->srcRegionID);
                            auto destRegion = context->regionCtrl.get_region(request->destRegionID);
            
                            transfer_to_region(request, response, srcRegion, destRegion, outputBuffer, socket, context);
                            result = message::status::ok;
                        } else {
                            auto destRegion = context->regionCtrl.get_region(request->destRegionID);
                            auto srcRegion = context->regionCtrl.get_region(request->srcRegionID);
                            
                            transfer_to_region(request, response, srcRegion, destRegion, outputBuffer, socket, context);
                            result = message::status::ok;
                        }




//                         if (obj != nullptr) {
//
//                             response->obj = *obj;
//
//                             for (auto& destUserID : region->get_users()) {
//                                 if (destUserID != request->accountID) {
//                                     auto info = context->userCtrl.get_info(destUserID);
//                                     socket.send_pkt(info->udpAddr, outputBuffer);
//                                 }
//                             }
//
//                             result = message::status::ok;
//                         }
                    } else {
                        result = message::status::error::auth;
                    }
                }

                header.status = result;
                return result;
            }

            static message::msg_status_t responseHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                return message::status::error::unknown;
            }
        };
    }
}
