#include "src/game/controller.h"

game::controller::controller(
    user_interface::controller& interfaceCtrl,
    util::event_controller<types::game_events>& eventCtrl,
    config::controller& config
)
: m_renderer(interfaceCtrl.renderer()),
  m_meshFactory(m_renderer),
  m_eventCtrl(eventCtrl),
  m_config(config)    
{
}

game::controller::~controller() {
}
