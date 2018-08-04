#pragma once

#include <gtkmm.h>
#include "src/types/game_events.h"

namespace user_interface {
    namespace views {
        class base_view {
        public:
            virtual void open() = 0;
            virtual void close() = 0;
            virtual Gtk::Overlay& container() = 0;  
            virtual Gtk::Container& overlay() = 0;            
            virtual void event_callback(const types::game_events event) = 0;
            
            constexpr static const auto events = {types::game_events::clear};
            static void event_dispatcher(const types::game_events event, base_view* view);            
        private:
            static int event_callback_wrapper(void* arg);            
        };
    }
}
