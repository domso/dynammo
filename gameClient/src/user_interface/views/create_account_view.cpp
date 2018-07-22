#include "src/user_interface/views/create_account_view.h"

#include "src/encryption/keygen.h"

user_interface::views::create_account_view::create_account_view(view_controller& viewCtrl) : m_viewCtrl(viewCtrl) {
    m_VBox.set_vexpand(true);
    m_VBox.set_hexpand(true);
    
    m_VBox.add(m_Box);
    m_Box.show();
    
    m_Box.property_halign().set_value(Gtk::Align::ALIGN_CENTER);
    m_Box.property_valign().set_value(Gtk::Align::ALIGN_CENTER);
    
    
    m_Box.add(m_entry);
    m_entry.show();
    m_entry.property_width_chars().set_value(20);
    m_entry.property_placeholder_text().set_value("Username");    
    m_entry.set_editable(true);
    
    m_entry.signal_changed().connect(sigc::mem_fun(*this, &create_account_view::entry_update));
    
    
    m_createButton.set_label("Create");
    m_createButton.show();
    m_createButton.signal_clicked().connect(sigc::mem_fun(*this, &create_account_view::create_button_clicked));
    m_createButton.set_sensitive(false);
    m_Box.add(m_createButton);
    
    m_backButton.set_label("back");
    m_backButton.show();
    m_backButton.signal_clicked().connect(sigc::mem_fun(*this, &create_account_view::back_button_clicked));
    m_Box.add(m_backButton);
    
    
    
    
    
    
    m_Box.show();
}

void user_interface::views::create_account_view::open() {
    
}

void user_interface::views::create_account_view::close() {

}

Gtk::Container& user_interface::views::create_account_view::container() {
    return m_VBox;
}

void user_interface::views::create_account_view::back_button_clicked() {
    m_viewCtrl.return_to_last_view();
}

void user_interface::views::create_account_view::create_button_clicked() {
    m_Box.set_sensitive(false);
    
    auto keyPair = encryption::keygen::generate();
    keyPair.value().first.save("../../keys/test_public.pem");
    keyPair.value().second.save("../../keys/test_private.pem");
    
}

void user_interface::views::create_account_view::entry_update() {
    m_createButton.set_sensitive(m_entry.get_text_length() > 0);
}

