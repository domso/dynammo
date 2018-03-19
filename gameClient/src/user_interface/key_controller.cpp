#include "src/user_interface/key_controller.h"

#include <chrono>
#include "src/user_interface/key_bindings.h"


user_interface::key_controller::key_controller(util::event_controller<types::game_events>& eventCtrl) : m_eventCtrl(eventCtrl) {
    register_key_event<key_bindings::move_up>();
    register_key_event<key_bindings::move_left>();
    register_key_event<key_bindings::move_down>();
    register_key_event<key_bindings::move_right>();
    
    m_running.store(true);
    m_thread = std::thread(&key_controller::update, this);
}

user_interface::key_controller::~key_controller() {
    m_running.store(false);
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void user_interface::key_controller::key_was_pressed(uint32_t key) {
    std::lock_guard<std::mutex> lg(m_mutex);

    if (m_pressedKeys.count(key) == 0) {
        create_pressed_events(key);
        m_pressedKeys.insert(key);
    }
}

void user_interface::key_controller::key_was_released(uint32_t key) {
    std::lock_guard<std::mutex> lg(m_mutex);
    create_release_events(key);
    m_pressedKeys.erase(key);
}

void user_interface::key_controller::update() {
    while (m_running.load(std::memory_order_acquire)) {
        std::chrono::milliseconds duration(250);
        std::this_thread::sleep_for(duration);
        {
            std::lock_guard<std::mutex> lg(m_mutex);

            for (auto key : m_pressedKeys) {
                create_pressed_events(key);
            }
        }

    }
}

void user_interface::key_controller::create_pressed_events(uint32_t key) {
    auto result = m_eventsByPress.find(key);

    if (result != m_eventsByPress.end()) {
        m_eventCtrl.new_event(result->second);
    }
}

void user_interface::key_controller::create_release_events(uint32_t key) {
    auto result = m_eventsByRelease.find(key);

    if (result != m_eventsByRelease.end()) {
        m_eventCtrl.new_event(result->second);
    }
}

