#ifndef gameClient_message_context_h
#define gameClient_message_context_h

#include "src/connector/controller.h"
#include "src/message/auth.h"
#include "src/region/controller.h"

namespace message {
    class context {
    public:
        context(connector::controller& connCtrl, region::controller& regCtrl);
        
        message::auth::content authContent;
    private:
        region::controller& m_regCtrl;
    };
}

#endif
