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
    m_button.set_size_request(200, 5);

//     m_button.show();
//     m_VBox.add_overlay(m_button);


    m_sidebar.property_halign().set_value(Gtk::Align::ALIGN_START);
    m_sidebar.property_valign().set_value(Gtk::Align::ALIGN_END);

//     m_sidebar.set_margin_bottom(100);
//     m_sidebar.set_size_request(100,500);


    m_sidebar.set_vexpand(true);
    m_sidebar.set_hexpand(true);
    m_button2.set_vexpand(true);
    m_button2.set_hexpand(true);
    m_sidebar.show();
    m_VBox.add_overlay(m_sidebar);

//     m_scroll.add(m_button2);
    m_button2.set_label("Overlay");
    m_button2.show();


    m_sidebar.attach(m_entry, 0, 6, 5, 1);
    m_entry.set_width_chars(50);
    m_entry.show();
    m_entry.signal_activate().connect(sigc::mem_fun(*this, &main_game_view::chat_enter));
    m_entry.property_placeholder_text().set_value("Type Message");


    m_scroll.add(m_text);
    m_text.show();
    m_text.set_vexpand(true);
    m_text.set_hexpand(true);
    m_text.set_editable(false);
    m_text.set_cursor_visible(false);
    m_text.set_opacity(0.75);
    m_text.set_focus_on_click(false);
    m_text.get_buffer()->signal_changed().connect(sigc::mem_fun(*this, &main_game_view::chat_recv));

//
    m_scroll.show();
    m_sidebar.attach(m_scroll, 0, 0, 5, 5);
//     m_scroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
//     m_scroll.property_min_content_width().set_value(500);
//     m_scroll.property_min_content_height().set_value(500);
    m_scroll.set_size_request(500, 300);

}

void user_interface::views::main_game_view::open() {

}

void user_interface::views::main_game_view::close() {

}

void user_interface::views::main_game_view::event_callback(const types::game_events event, user_interface::views::base_view* view) {
    main_game_view& current = *(dynamic_cast<main_game_view*>(view));
    
    if (event == types::game_events::recv_chat_message) {
        if (current.m_text.get_buffer()->get_text().length() > 0) {
            current.m_text.get_buffer()->set_text(
                current.m_text.get_buffer()->get_text() +
                "\n" +
                current.m_viewCtrl.config().global().get<std::string>("last-chat-message").second
            );
        } else {
            current.m_text.get_buffer()->set_text(
                current.m_viewCtrl.config().global().get<std::string>("last-chat-message").second
            );
        }    
    }
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

void user_interface::views::main_game_view::chat_enter() {
    if (m_entry.get_text_length() > 0) {
        
        
        m_viewCtrl.config().global().set<std::string>("chat-message", m_entry.get_text());
        m_viewCtrl.create_event(types::game_events::enter_chat_message);        
    }

    m_entry.set_text("");
    m_GLArea.set_can_focus(true);
    m_GLArea.grab_focus();
}

void user_interface::views::main_game_view::chat_recv() {
    //TODO crashes because its called from wrong thread
    //--> check how to
//     auto adj = m_scroll.get_vadjustment();
//     adj->set_value(adj->get_upper());  
}
