#include "src/user_interface/view_controller.h"

user_interface::view_controller::view_controller(Gtk::Box& rootBox, util::event_controller<types::game_events>& eventCtrl, util::config_file& config) : m_rootBox(rootBox), m_eventCtrl(eventCtrl), m_config(config) {

}

void user_interface::view_controller::change_view(const view_list::views newView) {
    close_current_view();
    m_currentView = newView;
    open_current_view();
}

void user_interface::view_controller::create_event(const types::game_events event) {
    m_eventCtrl.new_event(event);
}

void user_interface::view_controller::set_option(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_options[key] = value;
}

std::string user_interface::view_controller::get_option(const std::string& key) {
    std::lock_guard<std::mutex> lg(m_mutex);
    return m_options[key];
}

util::config_file& user_interface::view_controller::config() {
    return m_config;
}

void user_interface::view_controller::open_current_view() {
    auto result = m_views.find(m_currentView.get());

    if (result != m_views.end()) {
        auto& view = *result->second;
        m_rootBox.add(view.container());
        view.open();
        view.container().show();
    }
}

void user_interface::view_controller::close_current_view() {
    auto result = m_views.find(m_currentView.get());

    if (result != m_views.end()) {
        auto& view = *result->second;
        view.close();
        view.container().hide();
        m_rootBox.remove(view.container());
    }
}
