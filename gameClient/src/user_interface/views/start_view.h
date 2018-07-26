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
            
            start_view(view_controller& viewCtrl);
            
            void open();
            void close();   
            static types::game_events event_callback(const types::game_events event, const uint64_t& arg, base_view* view);        
            Gtk::Container& container();
        private:
            void run_button_clicked();
            void create_button_clicked();
             
            view_controller& m_viewCtrl;
            Gtk::Overlay m_VBox;
            Gtk::Image m_background;
            
            
            Gtk::Box m_loginBox;
            Gtk::Button m_loginButton;
            
            Gtk::Button m_createAccountButton;
            
            Gtk::Entry m_entry;
        };
    }
}
