#pragma once

#include "network/udp_socket.h"

#include "src/util/binary.h"
#include "src/region/vec2.h"
#include "src/message/msg_header.h"
#include "src/message/msg_types.h"
#include "src/types/msg_transfer/content.h"
#include "src/connector/context.h"
#include "src/types/game_events.h"
#include "src/connector/verify_buffer.h"

namespace connector {
    namespace msg_transfer {
        class action {
        public:
            typedef ::types::msg_transfer::content::action content;
            constexpr static const auto id = content::id;

            static bool request(message::msg_header_t& header, network::ipv4_addr& destAddr, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, void*, connector::context* context) {
                return false;
            }

            static message::msg_status_t requestHandler(message::msg_header_t& header, network::ipv4_addr& srcAddr, network::pkt_buffer& inputBuffer, network::pkt_buffer& outputBuffer, network::udp_socket<network::ipv4_addr>& socket, message::msg_option_t& options, connector::context* context) {
                auto request = inputBuffer.get_next<content::types::request>();
                auto response = outputBuffer.push_next<content::types::response>();
                auto result = message::status::error::unknown;
                bool verified = false;

                if (request != nullptr && response != nullptr) {
                    {
                        auto info = context->userCtrl.get_info(request->accountID);
                        verified = verify_buffer(inputBuffer, *info.data());
                    }


                    if (verified) {
                        ::types::game_events action = (::types::game_events) request->actionID;
                        response->regionID = request->regionID;

                        region::vec3<float> positionOffset;
                        uint32_t destRegionID = request->regionID;
//TODO
                        
                        auto actionObj = context->actionCtrl.execute_action(request->regionID, request->objID, action);
                        if (actionObj) {
                            response->obj = actionObj.value();
                        }
                        
                        /*
                        {
                            auto region = context->regionCtrl.get_region(request->regionID);
                            region::dynamic_obj* obj = region->get_dynamic_obj(request->objID);

                            if (obj != nullptr) {
                                context->actionCtrl.execute_action(*obj, action);

                                if (obj->position.x < 0) {
                                    positionOffset.x = 127;
                                    obj->position.x = 0;
                                    
                                    region::vec2<int16_t> regPosition = region::vec2<int16_t>::cast_from<uint32_t>(destRegionID);
                                    regPosition.x -= 1;
                                    destRegionID = regPosition.cast_to<uint32_t>();
                                    
                                }

                                if (obj->position.y < 0) {
                                    positionOffset.y = 127;
                                    obj->position.y = 0;
                                    
                                    region::vec2<int16_t> regPosition = region::vec2<int16_t>::cast_from<uint32_t>(destRegionID);
                                    regPosition.y -= 1;
                                    destRegionID = regPosition.cast_to<uint32_t>();
                                }

                                if (obj->position.x > 127) {
                                    positionOffset.x = obj->position.x - 127;
                                    obj->position.x = 127;
                                    
                                    region::vec2<int16_t> regPosition = region::vec2<int16_t>::cast_from<uint32_t>(destRegionID);
                                    regPosition.x += 1;
                                    destRegionID = regPosition.cast_to<uint32_t>();
                                }

                                if (obj->position.y > 127) {
                                    positionOffset.y = obj->position.y - 127;
                                    obj->position.y = 127;
                                    
                                    region::vec2<int16_t> regPosition = region::vec2<int16_t>::cast_from<uint32_t>(destRegionID);
                                    regPosition.y += 1;
                                    destRegionID = regPosition.cast_to<uint32_t>();
                                }

                                response->obj = *obj;
                            }
                        }
                        */
//TODO
/*
                        if (positionOffset.x != 0 || positionOffset.y != 0) {                                
                            auto srcRegion = context->regionCtrl.get_region(request->regionID);
                            auto destRegion = context->regionCtrl.get_region(destRegionID);
                            region::dynamic_obj* obj = srcRegion->get_dynamic_obj(request->objID);
                            
                           
                            obj->position.x += positionOffset.x;
                            obj->position.y += positionOffset.y;
                            obj->position.z += positionOffset.z;
                                                       
                            
                            obj->id = destRegion->insert_new_dynamic_object(*obj);
                            srcRegion->remove_dynamic_object(request->objID);
                            response->obj = *obj;
                            response->regionID = destRegionID;

                            auto responseExtension = outputBuffer.push_next<content::types::response_extension>();

                            responseExtension->oldRegionID = request->regionID;
                            responseExtension->oldObjID = request->objID;

                            for (auto& UserID : srcRegion->get_users()) {
                                if (UserID != request->accountID) {
                                    auto info = context->userCtrl.get_info(UserID);
                                    socket.send_pkt(info->udpAddr, outputBuffer);
                                }
                            }

                            for (auto& UserID : destRegion->get_users()) {
                                if (UserID != request->accountID) {
                                    auto info = context->userCtrl.get_info(UserID);
                                    socket.send_pkt(info->udpAddr, outputBuffer);
                                }
                            }
                        }
*/
                        result = message::status::ok;
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
