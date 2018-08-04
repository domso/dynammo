#pragma once

#include <stdint.h>
#include "src/region/dynamic_obj.h"
#include "src/types/game_events.h"

namespace types {
    namespace msg_transfer {
        namespace content {            
            struct ids {
                enum {
                    auth,    
                    create_account,
                    enter_region,
                    leave_region,
                    region_action,
                    region_chat
                };          
            };

            struct auth {
                constexpr static const uint8_t id = ids::auth;

                struct types {
                    struct request {
                        uint32_t accountID;
                        uint32_t sessionID;
                    };

                    struct response {
                        //-
                    };
                };
            };
            
            struct create_account {
                constexpr static const uint8_t id = ids::create_account;

                struct types {
                    struct request {
                        
                    };

                    struct response {
                        uint32_t accountID;
                    };
                };
            };

            struct enter_region {
                constexpr static const uint8_t id = ids::enter_region;

                struct types {
                    struct request {
                        uint32_t accountID;
                        uint32_t regionID;                      
                    };

                    struct response {
                        //-
                    };
                };
            };

            struct leave_region {
                constexpr static const uint8_t id = ids::leave_region;

                struct types {
                    struct request {
                        uint32_t accountID;
                        uint32_t regionID;                      
                    };

                    struct response {
                        //-
                    };
                };
            };
            
            struct region_action {
                constexpr static const uint8_t id = ids::region_action;

                struct types {
                    struct request {
                        uint32_t sessionID;
                        uint32_t regionID; 
                        ::types::game_events actionID;                          
                    };

                    struct response {
                        uint32_t sessionID;
                        uint32_t regionID; 
                        ::types::game_events actionID; 
                    };
                };
            };
            
            struct region_chat {
                constexpr static const uint8_t id = ids::region_chat;

                struct types {
                    struct request {
                        uint32_t sessionID;
                        uint32_t regionID; 
                        uint8_t length;
                    };

                    struct response {
                        //-
                    };
                };
            };
        }
    }
}
