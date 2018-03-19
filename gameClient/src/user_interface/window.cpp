#include "src/user_interface/window.h"

user_interface::window::window(graphic::renderer& renderer, util::event_controller<types::game_events>& eventCtrl) : m_keyCtrl(eventCtrl), m_renderer(renderer) {
    set_title("GL Area");
    set_default_size(800, 600);
    
    m_VBox.property_margin() = 12;
    add(m_VBox);

    m_GLArea.set_hexpand(true);
    m_GLArea.set_vexpand(true);
    
    m_renderer.link_glarea(m_GLArea);
    
    m_GLArea.set_auto_render(true);
    m_VBox.add(m_GLArea);
    
    m_VBox.show();
    m_GLArea.show();
    
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
}

user_interface::window::~window() {

}

bool user_interface::window::on_key_press_event(GdkEventKey* event) {
    m_keyCtrl.key_was_pressed(event->keyval);
}

bool user_interface::window::on_key_release_event(GdkEventKey* event) {
    m_keyCtrl.key_was_released(event->keyval);
}

bool user_interface::window::on_delete_event(GdkEventAny* event) {
    m_renderer.close();
    return true;
}


