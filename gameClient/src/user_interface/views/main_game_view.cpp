#include "src/user_interface/views/main_game_view.h"

user_interface::views::main_game_view::main_game_view(view_controller& viewCtrl, graphic::renderer& renderer,  key_controller& keyCtrl) : m_viewCtrl(viewCtrl), m_keyCtrl(keyCtrl) {
    m_GLArea.set_hexpand(true);
    m_GLArea.set_vexpand(true);
    
    renderer.link_glarea(m_GLArea);
    
    m_GLArea.set_auto_render(true);
    m_VBox.add(m_GLArea);
    m_GLArea.show();
    
//     viewCtrl.register_view(*this);
    
    
    m_button.property_halign().set_value(Gtk::Align::ALIGN_CENTER);
    m_button.property_valign().set_value(Gtk::Align::ALIGN_END);
    
    m_button.set_margin_bottom(100);
    m_button.set_size_request(200,5);
    
//     m_button.show();
//     m_VBox.add_overlay(m_button);
    
    
    m_sidebar.property_halign().set_value(Gtk::Align::ALIGN_END);
    m_sidebar.property_valign().set_value(Gtk::Align::ALIGN_END);
    
//     m_sidebar.set_margin_bottom(100);
//     m_sidebar.set_size_request(100,500);
    
    
    m_sidebar.set_vexpand(true);
    m_sidebar.set_hexpand(true);
    m_button2.set_vexpand(true);
    m_button2.set_hexpand(true);
    m_sidebar.show();
    m_VBox.add_overlay(m_sidebar);
    
    
    
    m_sidebar.add(m_button2);
    m_button2.set_label("Overlay");
    m_button2.show();
    
    
}

void user_interface::views::main_game_view::open() {
    
}

void user_interface::views::main_game_view::close() {

}

void user_interface::views::main_game_view::event_callback(const types::game_events event, user_interface::views::base_view* view)
{
}

Gtk::Container& user_interface::views::main_game_view::container() {
    return m_VBox;
}

bool user_interface::views::main_game_view::on_key_press_event(GdkEventKey* event) {
    m_keyCtrl.key_was_pressed(event->keyval);
    std::cout << "asd" << std::endl;
    return false;
}

bool user_interface::views::main_game_view::on_key_release_event(GdkEventKey* event) {
    m_keyCtrl.key_was_released(event->keyval);
    return false;
}
