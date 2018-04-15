#include "src/graphic/controller.h"

graphic::controller::controller(int argc, char* argv[], util::event_controller<types::game_events>& eventCtrl, util::config_file& config) : m_argc(argc), m_argv(argv), m_eventCtrl(eventCtrl), m_config(config) {

}

graphic::controller::~controller() {
    wait_for_close();
}

void graphic::controller::open() {
    m_app = Gtk::Application::create(m_argc, m_argv);
    m_window = std::make_unique<user_interface::window>(m_renderer, m_eventCtrl, m_config);
    m_thread = std::thread(&graphic::controller::thread_main, this);
}

void graphic::controller::wait_for_close() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

std::string graphic::controller::get_option(const std::string& key) {
    if (m_window.get() != nullptr) {
        return m_window->get_option(key);
    }
    
    return "";
}

void graphic::controller::clear() {
    m_texCtrl.clear();
    m_regionMeshes.clear();
    m_regionMeshes.clear();
}

void graphic::controller::thread_main() {
    assert(m_app);
    
    m_app->run(*m_window);
}
