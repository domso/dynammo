#pragma once

#include <stdint.h>
#include "src/authentication/types.h"
#include "src/message/msg_types.h"

namespace types {
    namespace msg_transfer {
        namespace content {            
            struct ids {
                enum {
                    info,
                    auth,
                    action
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
                        uint32_t actionID;
                    };

                    struct response {
                        //-
                    };
                };
            };
        }
    }
}
