#pragma once

#include <gtkmm.h>
#include <stdint.h>
#include <unordered_map>
#include <mutex>

#include "src/config/controller.h"
#include "src/util/event_controller.h"
#include "src/user_interface/window.h"
#include "src/graphic/renderer.h"

namespace user_interface {
    class controller {
    public:
        controller(int argc, char* argv[], util::event_controller<types::game_events>& eventCtrl, config::controller& config);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();
        
        void update();
        void close();
        
        graphic::renderer& renderer();
    private:        
        int m_argc;
        char** m_argv;
        std::mutex m_mutex;
        
        util::event_controller<types::game_events>& m_eventCtrl;           
        config::controller& m_config;
        
        graphic::renderer m_renderer;
        Glib::RefPtr<Gtk::Application> m_app;
    };
}
