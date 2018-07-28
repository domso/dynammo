#include "src/connector/context.h"

connector::context::context(
    region::controller& regCtrl,
    graphic::controller& graphicCtrl,
    util::config_file& config,
    util::event_controller<types::game_events>& eventCtrl
) : 
    regionCtrl(regCtrl),
    graphicCtrl(graphicCtrl),
    config(config),
    eventCtrl(eventCtrl) 
{

}
