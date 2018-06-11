#ifndef gameServer_connector_context_h
#define gameServer_connector_context_h

#include "src/connection/controller.h"
#include "src/region/controller.h"
#include "src/user/controller.h"
#include "src/action/controller.h"

namespace connector {
    class context {
    public:
        context(region::controller& rCtrl, action::controller& aCtrl, user::controller& uCtrl, connection::controller& cCtrl);
        
        region::controller& regionCtrl;    
        action::controller& actionCtrl;        
        
        user::controller& userCtrl;
        connection::controller& connectionCtrl;        
    };
}

#endif
