#pragma once

#include "src/region/controller.h"
#include "src/session/controller.h"

namespace connector {
    namespace msg_transfer {
        struct data {
            data(region::controller& regCtrl, session::controller& sessionCtrl);
            region::controller& regionCtrl;
            session::controller& sessionCtrl;
        };
    }
}
