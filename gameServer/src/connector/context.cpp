#include "src/connector/context.h"

connector::context::context(region::controller& rCtrl, session::controller& sCtrl) :
regionCtrl(rCtrl), sessionCtrl(sCtrl)
{
}

