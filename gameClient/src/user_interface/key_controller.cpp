#include "src/user_interface/key_controller.h"

#include "src/user_interface/key_bindings.h"

user_interface::key_controller::key_controller(util::event_controller<types::game_events>& eventCtrl) : m_eventCtrl(eventCtrl) {
    register_key_event<key_bindings::move_up>();
    register_key_event<key_bindings::move_left>();
    register_key_event<key_bindings::move_down>();
    register_key_event<key_bindings::move_right>();
}

user_interface::key_controller::~key_controller() {
}

void user_interface::key_controller::key_was_pressed(uint32_t key) {
    auto result = m_eventsByPress.find(key);
    if (result != m_eventsByPress.end()) {
        m_eventCtrl.new_event(result->second);
    }
}

void user_interface::key_controller::key_was_released(uint32_t key) {
    auto result = m_eventsByRelease.find(key);
    if (result != m_eventsByRelease.end()) {
        m_eventCtrl.new_event(result->second);
    }
}

