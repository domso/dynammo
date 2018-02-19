#include "src/message/context.h"

message::context::context(connector::controller& ctrl) {
    ctrl.register_message_obj<message::auth>(authContent);
}



