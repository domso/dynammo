#include "src/connector/context.h"

connector::context::context(region::controller& rCtrl, action::controller& aCtrl, user::controller& uCtrl, connection::controller& cCtrl) :
regionCtrl(rCtrl), actionCtrl(aCtrl), userCtrl(uCtrl), connectionCtrl(cCtrl)
{
}

