#include "src/graphic/controller.h"

graphic::controller::controller(int argc, char* argv[]) : m_argc(argc), m_argv(argv) {
   
}

graphic::controller::~controller() {
    wait_for_close();
}

void graphic::controller::open() {
    m_app = Gtk::Application::create(m_argc, m_argv);
    m_thread = std::thread(&graphic::controller::thread_main, this);
    m_renderer.currentState.wait_for(renderer::states::realized);
}

void graphic::controller::wait_for_close() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void graphic::controller::thread_main() {
    assert(m_app);
    graphic::window newWindow(m_renderer);
    m_window = &newWindow;
    m_app->run(newWindow);
    m_window = nullptr;
}
