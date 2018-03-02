#include "src/message/context.h"

message::context::context(connector::controller& connCtrl, region::controller& regCtrl) : m_regCtrl(regCtrl) {
    connCtrl.register_message_obj<message::auth>(authContent);
}



