#include "src/connector/context.h"

connector::context::context(region::controller& rCtrl, user::controller& uCtrl, connection::controller& cCtrl) :
regionCtrl(rCtrl), userCtrl(uCtrl), connectionCtrl(cCtrl)
{
}

