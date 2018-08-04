#pragma once

#include <stdint.h>
#include "src/region/layer.h"
#include "src/region/static_obj.h"
#include "src/region/dynamic_obj.h"

namespace types {
    namespace data_transfer {
        namespace content {                     
            struct ids {
                enum uint8_t {
                    auth_ticket,
                    session_id,
                    region_select,
                    region_layer,
                    static_object,
                    dynamic_object,
                    region_chat
                };          
            };
            
            struct auth_ticket {
                constexpr static const uint8_t id = ids::auth_ticket;
                typedef uint32_t content;
            };
            
            struct session_id {
                constexpr static const uint8_t id = ids::session_id;
                typedef uint32_t content;
            };
            
            struct region_select {
                constexpr static const uint8_t id = ids::region_select;
                typedef uint32_t content;
            };
            
            struct region_layer {
                constexpr static const uint8_t id = ids::region_layer;
                typedef region::layer<uint32_t> content;
            };
                        
            struct static_object {
                constexpr static const uint8_t id = ids::static_object;
                typedef region::static_obj content;
            };
            
            struct dynamic_object {
                constexpr static const uint8_t id = ids::dynamic_object;
                typedef region::dynamic_obj content;
            };
            
            struct region_chat {
                constexpr static const uint8_t id = ids::region_chat;
                typedef char content;
            };
        }
    }
}
