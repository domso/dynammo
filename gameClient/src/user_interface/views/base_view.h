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
            
            constexpr static const auto events = {types::game_events::clear};
            static void event_callback(const types::game_events event, base_view* view);
        };
    }
}
