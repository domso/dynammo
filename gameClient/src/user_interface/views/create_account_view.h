#pragma once

#include <gtkmm.h>
#include "src/user_interface/view_list.h"
#include "src/user_interface/views/base_view.h"
#include "src/user_interface/view_controller.h"

namespace user_interface {
    namespace views {
        class create_account_view : public base_view {
        public:                        
            constexpr static const auto id = view_list::views::account_create;            
            create_account_view(view_controller& viewCtrl);
            
            void open();
            void close();            
            Gtk::Container& container();
        private:
            void back_button_clicked();
            void create_button_clicked();
            void entry_update();
            
            view_controller& m_viewCtrl;
            
            Gtk::Button m_backButton;
            Gtk::Button m_createButton;
            Gtk::Entry m_entry;
            Gtk::Overlay m_VBox;
            Gtk::Box m_Box;
        };
    }
}
