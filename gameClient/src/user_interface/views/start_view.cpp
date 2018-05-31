#include "src/user_interface/views/start_view.h"

user_interface::views::start_view::start_view(view_controller& viewCtrl) : m_viewCtrl(viewCtrl) {
    
//     m_background.set("../res/background.png");
//     m_background.set_vexpand(true);
//     m_background.set_hexpand(true);
//     m_background.show();
    
    
    m_VBox.add(m_background);
    m_VBox.set_vexpand(true);
    m_VBox.set_hexpand(true);
    
    m_loginBox.property_halign().set_value(Gtk::Align::ALIGN_CENTER);
    m_loginBox.property_valign().set_value(Gtk::Align::ALIGN_CENTER);
    
    m_VBox.add_overlay(m_loginBox);
    m_loginBox.show();
    
    m_loginBox.add(m_entry);
    m_loginBox.add(m_button);
    
    m_button.set_label("Start");
    m_button.show();
    
    m_button.signal_clicked().connect( sigc::mem_fun(*this, &start_view::run_button_clicked) );
    
    m_entry.show();
    m_entry.property_width_chars().set_value(20);
    m_entry.property_placeholder_text().set_value("Username");    
    m_entry.set_editable(true);

    viewCtrl.register_view(*this);
}

void user_interface::views::start_view::open() {
    
}

void user_interface::views::start_view::close() {

}

Gtk::Container& user_interface::views::start_view::container() {
    return m_VBox;
}

void user_interface::views::start_view::run_button_clicked() {
    m_viewCtrl.config().clear();
    m_viewCtrl.config().load("../" + m_entry.get_text() + ".config");   
    
    m_viewCtrl.create_event(types::game_events::request_login);    
    m_viewCtrl.change_view(view_list::views::main_game);
}
