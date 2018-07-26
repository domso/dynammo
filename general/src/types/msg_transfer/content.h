#pragma once

#include <stdint.h>
#include "src/authentication/types.h"
#include "src/message/msg_types.h"
#include "src/region/dynamic_obj.h"


namespace types {
    namespace msg_transfer {
        namespace content {            
            struct ids {
                enum {
                    info,
                    auth,
                    action,    
                    transfer_action,                    
                    enter_region,
                    leave_region,
                    create_account
                };          
            };
            
            struct info {
                constexpr static const uint8_t id = ids::info;

                struct types {
                    struct request {
                        authentication::regionID_t regID;
                    };

                    struct response {
                        authentication::regionID_t regID;
                        uint16_t port;
                    };
                };
            };

            struct auth {
                constexpr static const uint8_t id = ids::auth;

                struct types {
                    struct request {
                        authentication::accountID_t accountID;
                        authentication::ticket_t tcpTicket;
                    };

                    struct response {
                        authentication::accountID_t accountID;
                    };
                };
            };

            struct action {
                constexpr static const uint8_t id = ids::action;

                struct types {
                    struct request {
                        authentication::accountID_t accountID;
                        uint32_t regionID;
                        uint32_t objID;
                        uint32_t actionID;                        
                    };

                    struct response {
                        uint32_t regionID;
                        region::dynamic_obj obj;
                    };
                    
                    struct response_extension {
                        uint32_t oldRegionID;
                        uint32_t oldObjID;
                    };
                };
            };

            struct transfer_action {
                constexpr static const uint8_t id = ids::transfer_action;

                struct types {
                    struct request {
                        authentication::accountID_t accountID;
                        uint32_t srcRegionID;
                        uint32_t destRegionID;
                        uint32_t objID;
                        uint32_t actionID;                        
                    };

                    struct response {
                        uint32_t srcRegionID;
                        uint32_t oldObjID;
                        uint32_t destRegionID;
                        region::dynamic_obj obj;
                    };
                };
            };

            struct enter_region {
                constexpr static const uint8_t id = ids::enter_region;

                struct types {
                    struct request {
                        authentication::accountID_t accountID;
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
                        authentication::accountID_t accountID;
                        uint32_t regionID;                      
                    };

                    struct response {
                        //-
                    };
                };
            };
        }
    }
}
