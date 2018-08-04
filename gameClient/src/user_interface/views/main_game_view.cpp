#include "src/user_interface/views/main_game_view.h"

user_interface::views::main_game_view::main_game_view(
    view_controller& viewCtrl,
    graphic::renderer& renderer,
    key_controller& keyCtrl
) : m_viewCtrl(viewCtrl), m_keyCtrl(keyCtrl) {
    m_GLArea.set_hexpand(true);
    m_GLArea.set_vexpand(true);

    renderer.link_glarea(m_GLArea);

    m_GLArea.set_auto_render(true);
    m_VBox.add(m_GLArea);
    m_GLArea.show();    
}

void user_interface::views::main_game_view::open() {

}

void user_interface::views::main_game_view::close() {

}

void user_interface::views::main_game_view::event_callback(const types::game_events event) {
    if (event == types::game_events::chat_close) {
        m_GLArea.set_can_focus(true);
        m_GLArea.grab_focus();
    }
}

Gtk::Overlay& user_interface::views::main_game_view::container() {
    return m_VBox;
}

Gtk::Container& user_interface::views::main_game_view::overlay() {
    return m_container;
}
