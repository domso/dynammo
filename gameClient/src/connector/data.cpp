#include "src/connector/data.h"

connector::msg_transfer::data::data(region::controller& regCtrl, session::controller& sessionCtrl, util::config_file& config, util::event_controller<types::game_events>& eventCtrl) : regionCtrl(regCtrl), sessionCtrl(sessionCtrl), config(config), eventCtrl(eventCtrl) {

}
