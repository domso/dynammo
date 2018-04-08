#pragma once

#include <gtkmm.h>

namespace user_interface {
    namespace views {
        class base_view {
        public:
            virtual void open() = 0;
            virtual void close() = 0;
            virtual Gtk::Container& container() = 0;
        };
    }
}
