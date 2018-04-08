#pragma once

#include <gtkmm.h>
#include <unordered_map>
#include "src/util/state_machine.h"
#include "src/user_interface/view_list.h"
#include "src/user_interface/views/base_view.h"

namespace user_interface {
    class view_controller {
    public:
        view_controller(Gtk::Box& rootBox);
        
        template <typename T>
        void register_view(T& newView) {   
            if (m_currentView == T::id) {
                close_current_view();
                m_views[T::id] = dynamic_cast<views::base_view*>(&newView);
                open_current_view();
            } else {
                m_views[T::id] = dynamic_cast<views::base_view*>(&newView);
            }
        }
        
        void change_view(const view_list::views newView);
        
    private:        
        void open_current_view();        
        void close_current_view();
        
        Gtk::Box& m_rootBox;
        std::unordered_map<view_list::views, views::base_view*> m_views;
        util::state_machine<view_list::views> m_currentView = view_list::views::start;
    };
}
