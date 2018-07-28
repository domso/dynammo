#pragma once

#include <gtkmm.h>
#include "src/user_interface/key_controller.h"
#include "src/user_interface/view_list.h"
#include "src/user_interface/views/base_view.h"
#include "src/user_interface/view_controller.h"
#include "src/graphic/renderer.h"

namespace user_interface {
    namespace views {
        class main_game_view : public base_view {
        public:                        
            constexpr static const auto id = view_list::views::main_game;
            
            main_game_view(view_controller& viewCtrl, graphic::renderer& renderer, key_controller& keyCtrl);
            
            void open();
            void close();            
            static void event_callback(const types::game_events event, base_view* view);
            Gtk::Container& container();            
        private:
            bool on_key_press_event(GdkEventKey* event);
            bool on_key_release_event(GdkEventKey* event);
            
            view_controller& m_viewCtrl;
            key_controller& m_keyCtrl;
            
            
            Gtk::Overlay m_VBox;
            Gtk::GLArea m_GLArea;  
            Gtk::Button m_button;
            
            Gtk::Box m_sidebar;
            Gtk::Button m_button2;
        };
    }
}

