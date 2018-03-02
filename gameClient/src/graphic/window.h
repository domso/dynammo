#ifndef gameClient_graphic_window_h
#define gameClient_graphic_window_h

#include <gtkmm.h>
#include <epoxy/gl.h>
#include <glm/glm.hpp>

#include "src/graphic/renderer.h"

namespace graphic {
    class window : public Gtk::Window {
    public:
        window(graphic::renderer& renderer);
        ~window() override;
    protected:
        graphic::renderer& m_renderer;
        Gtk::Box m_VBox ;
        Gtk::GLArea m_GLArea;
    };
}

#endif

