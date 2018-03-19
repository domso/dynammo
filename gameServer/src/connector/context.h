#ifndef gameServer_connector_context_h
#define gameServer_connector_context_h

#include "src/connection/controller.h"
#include "src/region/controller.h"
#include "src/user/controller.h"

namespace connector {
    class context {
    public:
        context(region::controller& rCtrl, user::controller& uCtrl, connection::controller& cCtrl);
        
        region::controller& regionCtrl;    
        user::controller& userCtrl;
        connection::controller& connectionCtrl;        
    };
}

#endif
