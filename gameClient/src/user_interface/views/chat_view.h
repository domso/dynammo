#pragma once

#include <gtkmm.h>
#include "src/user_interface/key_controller.h"
#include "src/user_interface/view_list.h"
#include "src/user_interface/views/base_view.h"
#include "src/user_interface/view_controller.h"
#include "src/graphic/renderer.h"
#include <queue>

namespace user_interface {
    namespace views {
        class chat_view : public base_view {
        public:                        
            constexpr static const auto id = view_list::views::main_game;
            constexpr static const auto events = {types::game_events::recv_chat_message};
            chat_view(view_controller& viewCtrl, graphic::renderer& renderer, key_controller& keyCtrl);
            
            void open();
            void close();            
            void event_callback(const types::game_events event);
            Gtk::Overlay& container();     
            Gtk::Container& overlay();           
        private:          
            void chat_enter();
            void chat_recv();
            
            void insert_all_messages();
            void scroll_down();
            
            view_controller& m_viewCtrl;
            key_controller& m_keyCtrl;

            std::queue<std::string> m_receivedMessages;
                        
            Gtk::Overlay m_emptyOverlay;
            
            Gtk::Grid m_chatBox;
            Gtk::Entry m_msgEntry;            
            Gtk::ScrolledWindow m_msgWindow;
            Gtk::TextView m_msgView;
        };
    }
}


