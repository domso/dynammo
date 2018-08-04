#include "src/connector/context.h"

connector::context::context(
    game::controller& gameCtrl,
    util::event_controller<types::game_events>& eventCtrl,
    config::controller& config
) : 
    gameCtrl(gameCtrl),
    eventCtrl(eventCtrl),
    config(config)
{

}
