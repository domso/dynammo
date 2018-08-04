#pragma once

#include <gtkmm.h>
#include <mutex>
#include <unordered_map>
#include <iostream>

#include "src/config/controller.h"
#include "src/util/state_machine.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/user_interface/view_list.h"
#include "src/user_interface/views/base_view.h"

namespace user_interface {
    class view_controller {
    public:
        view_controller(Gtk::Box& rootBox, util::event_controller<types::game_events>& eventCtrl, config::controller& config, Gtk::Window& window);
        view_controller(const view_controller& copy) = delete;
        view_controller(view_controller&& move) = delete;
        ~view_controller();
        
        template <typename T, typename ...Ta>
        void register_view(Ta... args) {   
            views::base_view* newView = dynamic_cast<views::base_view*>(new T(*this, args...));
            
            if (m_views[T::id].size() == 0) {
                m_rootBox.add(newView->container());                
            } else {
                m_views[T::id][0]->container().add_overlay(newView->overlay());
            }
            
            if (m_currentView == T::id) {
                close_current_view();
                m_views[T::id].push_back(std::unique_ptr<views::base_view>(newView));
                open_current_view();
            } else {
                m_views[T::id].push_back(std::unique_ptr<views::base_view>(newView));
            }
            
            for (auto e : T::events) {
                m_eventCtrl.register_event_handler(e, T::event_dispatcher, newView);
                m_allRegisteredEvents.push_back(e);
            }
        }
        
        void change_view(const view_list::views newView);
        void return_to_last_view();
        void create_event(const types::game_events event);        
        
        void set_option(const std::string& key, const std::string& value);
        std::string get_option(const std::string& key);
        
        config::controller& config();
        Gtk::Window& window();
    private:        
        void open_current_view();        
        void close_current_view();
        
        std::mutex m_mutex;
        Gtk::Box& m_rootBox;
        std::vector<view_list::views> m_viewHistory;
        
        std::vector<types::game_events> m_allRegisteredEvents;
        std::unordered_map<view_list::views, std::vector<std::unique_ptr<views::base_view>>> m_views;
        util::state_machine<view_list::views> m_currentView = view_list::views::start;
        util::event_controller<types::game_events>& m_eventCtrl;
        std::unordered_map<std::string, std::string> m_options;
        config::controller& m_config;
        Gtk::Window& m_window;
    };
}
