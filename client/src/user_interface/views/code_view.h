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
        class code_view : public base_view {
        public:                        
            constexpr static const auto id = view_list::views::main_game;
            code_view(view_controller& viewCtrl, graphic::renderer& renderer, key_controller& keyCtrl);
            
            void open();
            void close();            
            void event_callback(const types::game_events event);
            Gtk::Overlay& container();     
            Gtk::Container& overlay();           
        private: 
            void assemble();
            
            std::vector<uint32_t> m_currentCode;
            
            view_controller& m_viewCtrl;
            key_controller& m_keyCtrl;

            Gtk::Overlay m_emptyOverlay;
            
            Gtk::Button m_acceptButton;
            Gtk::Grid m_chatBox;     
            
            Gtk::ScrolledWindow m_assemblyWindow;
            Gtk::TextView m_assembly;
            
            Gtk::ScrolledWindow m_binaryWindow;
            Gtk::TextView m_binary;
        };
    }
}



