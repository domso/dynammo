#ifndef gameClient_message_context_h
#define gameClient_message_context_h

#include "src/connector/controller.h"
#include "src/message/auth.h"

namespace message {
    class context {
    public:
        context(connector::controller& ctrl);
        
        message::auth::content authContent;
    };
}

#endif
