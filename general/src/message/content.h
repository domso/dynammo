#ifndef general_message_content_h
#define general_message_content_h

#include <stdint.h>
#include "src/authentication/types.h"
#include "src/message/msg_types.h"
#include "src/util/state_machine.h"
#include "network/pkt_buffer.h"

namespace message {
    namespace content {
        struct info {
            constexpr static const uint8_t id = 0;

            struct types {
                struct request {
                    authentication::regionID_t regID;
                };

                struct response {
                    authentication::regionID_t regID;
                    uint16_t port;
                };
            };

            types::request request;
            types::response response;
            message::msg_status_t status;
            util::state_machine<message::types::states> currentState;
        };

        struct auth {
            constexpr static const uint8_t id = 1;

            struct types {
                struct request {
                    authentication::accountID_t accountID;
                    int32_t tcpTicket;
                };

                struct response {
                    authentication::accountID_t accountID;
                };
            };

            types::request request;
            types::response response;
            message::msg_status_t status;
            util::state_machine<message::types::states> currentState;
        };
    }
}
#endif
