#include "src/user_interface/controller.h"

user_interface::controller::controller(int argc, char* argv[], util::event_controller<types::game_events>& eventCtrl, config::controller& config) : m_argc(argc), m_argv(argv), m_eventCtrl(eventCtrl), m_config(config) {

}

user_interface::controller::~controller() {

}

void user_interface::controller::update() {
    m_app = Gtk::Application::create(m_argc, m_argv);
    auto window = std::make_unique<user_interface::window>(m_renderer, m_eventCtrl, m_config);
    m_app->run(*window);
}

void user_interface::controller::close() {
    
}

graphic::renderer& user_interface::controller::renderer() {
    return m_renderer;
}
