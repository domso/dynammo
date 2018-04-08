#include "src/graphic/controller.h"

graphic::controller::controller(int argc, char* argv[], util::event_controller<types::game_events>& eventCtrl) : m_argc(argc), m_argv(argv), m_eventCtrl(eventCtrl) {

}

graphic::controller::~controller() {
    wait_for_close();
}

void graphic::controller::open() {
    m_app = Gtk::Application::create(m_argc, m_argv);
    m_thread = std::thread(&graphic::controller::thread_main, this);
}

void graphic::controller::wait_for_close() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void graphic::controller::clear() {
    m_texCtrl.clear();
    m_regionMeshes.clear();
    m_regionMeshes.clear();
}

void graphic::controller::thread_main() {
    assert(m_app);
    user_interface::window newWindow(m_renderer, m_eventCtrl);
    m_app->run(newWindow);
    m_app->remove_window(newWindow);
}
