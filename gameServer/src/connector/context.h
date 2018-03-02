#ifndef gameServer_connector_context_h
#define gameServer_connector_context_h

#include "src/connection/controller.h"
#include "src/region/controller.h"
#include "src/user/controller.h"

namespace connector {
    class context {
    public:
        context() : m_connectionCtrl(1850), m_regionCtrl(m_userCtrl) {
            
        }
        
        connection::controller m_connectionCtrl;
        user::controller m_userCtrl;
        region::controller m_regionCtrl;        
    };
}

#endif
