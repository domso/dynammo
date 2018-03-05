#ifndef gameClient_user_interface_window_h
#define gameClient_user_interface_window_h

#include <gtkmm.h>
#include <epoxy/gl.h>
#include <glm/glm.hpp>

#include "src/graphic/renderer.h"
#include "src/user_interface/key_controller.h"

namespace user_interface {
    class window : public Gtk::Window {
    public:
        window(graphic::renderer& renderer, util::event_controller<types::game_events>& eventCtrl);
        ~window() override;
        
        bool on_key_press_event(GdkEventKey* event) override;
        bool on_key_release_event(GdkEventKey* event) override;
    protected:
        key_controller m_keyCtrl;
        graphic::renderer& m_renderer;
        Gtk::Box m_VBox ;
        Gtk::GLArea m_GLArea;
    };
}

#endif

