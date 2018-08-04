#include "src/user_interface/views/create_account_view.h"

#include "src/encryption/keygen.h"


user_interface::views::create_account_view::create_account_view(view_controller& viewCtrl) : m_viewCtrl(viewCtrl) {
    m_VBox.set_vexpand(true);
    m_VBox.set_hexpand(true);
    
    m_VBox.add(m_grid);
    m_grid.show();
    
    m_grid.property_halign().set_value(Gtk::Align::ALIGN_CENTER);
    m_grid.property_valign().set_value(Gtk::Align::ALIGN_CENTER);
    
        
    m_grid.attach(m_entry, 0, 0, 5, 1);
    
    m_entry.show();
    m_entry.property_width_chars().set_value(20);
    m_entry.property_placeholder_text().set_value("Username");    
    m_entry.set_editable(true);
    
    m_entry.signal_changed().connect(sigc::mem_fun(*this, &create_account_view::entry_update));
    
    
    m_createButton.set_label("Create");
    m_createButton.show();
    m_createButton.signal_clicked().connect(sigc::mem_fun(*this, &create_account_view::create_button_clicked));
    m_createButton.set_sensitive(false);
    m_grid.attach(m_createButton, 6, 0, 1, 1);
    
    m_backButton.set_label("back");
    m_backButton.show();
    m_backButton.signal_clicked().connect(sigc::mem_fun(*this, &create_account_view::back_button_clicked));
    m_grid.attach(m_backButton, 7, 0, 1, 1);
    
//     m_progress.show();
    m_grid.attach(m_progress, 0, 1, 8, 1);
    

    m_grid.show();
}

void user_interface::views::create_account_view::open() {
    
}

void user_interface::views::create_account_view::close() {

}

void user_interface::views::create_account_view::event_callback(const types::game_events event, user_interface::views::base_view* view)
{
    create_account_view& current = *(dynamic_cast<create_account_view*>(view));
    
    if (event == types::game_events::success_account_creation) {
        current.m_progress.set_fraction(1);
        current.m_progress.set_text("Success");
        current.m_viewCtrl.config().global().store("../" + current.m_viewCtrl.config().global().get<std::string>("username").second + ".config");
    } else {
        current.m_progress.set_fraction(1);
        current.m_progress.set_text("Error");
    }
    
    current.m_createButton.set_sensitive(true);
    current.m_backButton.set_sensitive(true);
    current.m_entry.set_sensitive(true);
    current.m_entry.set_text("");
}

Gtk::Overlay& user_interface::views::create_account_view::container() {
    return m_VBox;
//     return m_grid;
}


Gtk::Container& user_interface::views::create_account_view::overlay() {
    return m_container;
}

void user_interface::views::create_account_view::back_button_clicked() {
    m_progress.hide();
    m_viewCtrl.return_to_last_view();
}

void user_interface::views::create_account_view::create_button_clicked() {
    m_createButton.set_sensitive(false);
    m_backButton.set_sensitive(false);
    m_entry.set_sensitive(false);
    
    m_progress.show();
    m_progress.set_fraction(0);
    m_progress.set_show_text("");
    m_progress.set_text("Creating keys");   
    
    auto keyPair = encryption::keygen::generate();
    
    m_progress.set_fraction(0.25);
    m_progress.set_text("Saving keys");
    
    keyPair.value().first.save("../../keys/" + m_entry.get_text() + "_public.pem");
    keyPair.value().second.save("../../keys/" + m_entry.get_text() + "_private.pem");

    m_progress.set_fraction(0.5);
    m_progress.set_text("Send request");
    
    m_viewCtrl.config().global().set<std::string>("username", std::string(m_entry.get_text()));
    m_viewCtrl.config().global().set<std::string>("privateKey", "../../keys/" + m_entry.get_text() + "_private.pem");
    m_viewCtrl.create_event(types::game_events::request_account_creation);
    
    m_progress.set_fraction(0.75);
    m_progress.set_text("Wait for response");
}

void user_interface::views::create_account_view::entry_update() {
    m_createButton.set_sensitive(m_entry.get_text_length() > 0);
}

