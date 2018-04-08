#ifndef gameClient_user_interface_window_h
#define gameClient_user_interface_window_h

#include <gtkmm.h>
#include <epoxy/gl.h>
#include <glm/glm.hpp>

#include "src/graphic/renderer.h"
#include "src/util/state_machine.h"
#include "src/user_interface/key_controller.h"
#include "src/user_interface/view_controller.h"
#include "src/user_interface/views/start_view.h"
#include "src/user_interface/views/main_game_view.h"

namespace user_interface {
    class window : public Gtk::Window {
    public:
        window(graphic::renderer& renderer, util::event_controller<types::game_events>& eventCtrl);
        ~window() override;
        
        enum class states {       
            open,
            closed
        };
        
        bool key_press_event(GdkEventKey* event) ;
        bool key_release_event(GdkEventKey* event);
        bool on_delete_event(GdkEventAny* event) override;
    protected:        
        key_controller m_keyCtrl;
        graphic::renderer& m_renderer;
        Gtk::Box m_rootBox;
        
        view_controller m_viewCtrl;
        views::start_view m_startView;
        views::main_game_view m_mainGameView;
    };
}

#endif

