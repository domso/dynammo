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
    m_loginBox.add(m_loginButton);
    m_loginBox.add(m_createAccountButton);
    
    m_loginButton.set_label("Login");
    m_loginButton.show();
    m_loginButton.signal_clicked().connect( sigc::mem_fun(*this, &start_view::run_button_clicked) );
    
    m_createAccountButton.set_label("Create Account");
    m_createAccountButton.show();
    m_createAccountButton.signal_clicked().connect(sigc::mem_fun(*this, &start_view::create_button_clicked));
    
    
    
    m_entry.show();
    m_entry.property_width_chars().set_value(20);
    m_entry.property_placeholder_text().set_value("Username");    
    m_entry.set_editable(true);
}

void user_interface::views::start_view::open() {
    
}

void user_interface::views::start_view::close() {

}

void user_interface::views::start_view::event_callback(const types::game_events event) {
    if (event == types::game_events::success_login) {  
        m_VBox.set_sensitive(true);
        m_viewCtrl.create_event(types::game_events::enter_region);
        m_viewCtrl.change_view(view_list::views::main_game);
    } else {
        m_VBox.set_sensitive(true);
        
        Gtk::MessageDialog errorDialog(m_viewCtrl.window(), "Error " + m_viewCtrl.config().global().get<std::string>("Auth-State").second);                                                                                                          
        errorDialog.run();
    }
}

Gtk::Overlay& user_interface::views::start_view::container() {
    return m_VBox;
}

Gtk::Container& user_interface::views::start_view::overlay() {
    return m_loginBox   ;
}

void user_interface::views::start_view::run_button_clicked() {
    if (m_viewCtrl.config().global().load("../" + m_entry.get_text() + ".config")) {
        m_VBox.set_sensitive(false);    
        m_viewCtrl.create_event(types::game_events::request_auth);
    } else {
        Gtk::MessageDialog errorDialog(m_viewCtrl.window(), "Could not open account-file for '" + m_entry.get_text() + "'");
        errorDialog.run();
    }    
}

void user_interface::views::start_view::create_button_clicked() {
    m_viewCtrl.change_view(view_list::views::account_create);
}
