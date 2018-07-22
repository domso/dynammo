#pragma once

#include <gtkmm.h>
#include <mutex>
#include <unordered_map>

#include "src/util/config_file.h"
#include "src/util/state_machine.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/user_interface/view_list.h"
#include "src/user_interface/views/base_view.h"

namespace user_interface {
    class view_controller {
    public:
        view_controller(Gtk::Box& rootBox, util::event_controller<types::game_events>& eventCtrl, util::config_file& config);
        
        template <typename T, typename ...Ta>
        void register_view(Ta... args) {   
            if (m_currentView == T::id) {
                close_current_view();
                m_views[T::id] = std::unique_ptr<views::base_view>(dynamic_cast<views::base_view*>(new T(*this, args...)));
                open_current_view();
            } else {
                m_views[T::id] = std::unique_ptr<views::base_view>(dynamic_cast<views::base_view*>(new T(*this, args...)));
            }
        }
        
        void change_view(const view_list::views newView);
        void return_to_last_view();
        void create_event(const types::game_events event);
        void set_option(const std::string& key, const std::string& value);
        std::string get_option(const std::string& key);
        
        util::config_file& config();
    private:        
        void open_current_view();        
        void close_current_view();
        
        std::mutex m_mutex;
        Gtk::Box& m_rootBox;
        std::vector<view_list::views> m_viewHistory;
        std::unordered_map<view_list::views, std::unique_ptr<views::base_view>> m_views;
        util::state_machine<view_list::views> m_currentView = view_list::views::start;
        util::event_controller<types::game_events>& m_eventCtrl;
        std::unordered_map<std::string, std::string> m_options;
        util::config_file& m_config;
    };
}
