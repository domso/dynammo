#include "src/graphic/window.h"


graphic::window::window(graphic::renderer& renderer) : m_renderer(renderer) {
    set_title("GL Area");
    set_default_size(800, 600);
    
    m_VBox.property_margin() = 12;
//     m_VBox.set_spacing(6);
    add(m_VBox);

    m_GLArea.set_hexpand(true);
    m_GLArea.set_vexpand(true);
    
//     m_GLArea.set_size_request(400, 600);
    m_renderer.link_glarea(m_GLArea);
    
    m_GLArea.set_auto_render(true);
    m_VBox.add(m_GLArea);
    
    m_VBox.show();
    m_GLArea.show();
}

graphic::window::~window() {

}

