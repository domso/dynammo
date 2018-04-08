#include "src/user_interface/window.h"

user_interface::window::window(graphic::renderer& renderer, util::event_controller<types::game_events>& eventCtrl) :
    m_keyCtrl(eventCtrl),
    m_renderer(renderer),
    m_viewCtrl(m_rootBox),
    m_startView(m_viewCtrl),
    m_mainGameView(m_viewCtrl, renderer, m_keyCtrl)
{
    set_title("GL Area");
    set_default_size(800, 600);
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
//     m_VBox.property_margin() = 12;
    
    add(m_rootBox);   
    m_rootBox.show();   
    
    signal_key_press_event().connect(sigc::mem_fun(*this, &window::key_press_event), true);
    signal_key_release_event().connect(sigc::mem_fun(*this, &window::key_release_event), true);    
}

user_interface::window::~window() {

}

bool user_interface::window::key_press_event(GdkEventKey* event) {
    m_keyCtrl.key_was_pressed(event->keyval);
    return false;
}

bool user_interface::window::key_release_event(GdkEventKey* event) {
    m_keyCtrl.key_was_released(event->keyval);
    return false;
}

bool user_interface::window::on_delete_event(GdkEventAny* event) {
    m_renderer.close();
    return false;
}
