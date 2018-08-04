#pragma once

#include <gtkmm.h>
#include "src/user_interface/view_list.h"
#include "src/user_interface/views/base_view.h"
#include "src/user_interface/view_controller.h"

namespace user_interface {
    namespace views {
        class start_view : public base_view {
        public:                        
            constexpr static const auto id = view_list::views::start;
            constexpr static const auto events = 
            {
                types::game_events::success_login,                
                types::game_events::failure_login
            };
            
            start_view(view_controller& viewCtrl);
            
            void open();
            void close();   
            void event_callback(const types::game_events event);        
            Gtk::Overlay& container();   
            Gtk::Container& overlay(); 
        private:
            void run_button_clicked();
            void create_button_clicked();
             
            view_controller& m_viewCtrl;
            Gtk::Overlay m_VBox;
            Gtk::Image m_background;
            
            
            Gtk::Box m_loginBox;
            Gtk::Button m_loginButton;
            Gtk::Box m_container;
            
            Gtk::Button m_createAccountButton;
            
            Gtk::Entry m_entry;
        };
    }
}
