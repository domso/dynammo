#ifndef gameServer_connector_context_h
#define gameServer_connector_context_h

#include "src/region/controller.h"
#include "src/session/controller.h"

namespace connector {
    class context {
    public:
        context(region::controller& rCtrl, session::controller& sCtrl);
        
        region::controller& regionCtrl;      
        session::controller& sessionCtrl;       
    };
}

#endif
