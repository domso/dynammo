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
            constexpr static const auto events = 
            {
                types::game_events::chat_close                
            };
            main_game_view(view_controller& viewCtrl, graphic::renderer& renderer, key_controller& keyCtrl);
            
            void open();
            void close();            
            static void event_callback(const types::game_events event, base_view* view);
            Gtk::Overlay& container();    
            Gtk::Container& overlay();              
        private:                        
            view_controller& m_viewCtrl;
            key_controller& m_keyCtrl;
            Gtk::Box m_container;

            Gtk::Overlay m_VBox;
            Gtk::GLArea m_GLArea;            
        };
    }
}

