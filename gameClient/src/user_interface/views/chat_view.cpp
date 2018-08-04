#include "src/user_interface/views/chat_view.h"

user_interface::views::chat_view::chat_view(
    view_controller& viewCtrl,
    graphic::renderer& renderer,
    key_controller& keyCtrl
) : m_viewCtrl(viewCtrl), m_keyCtrl(keyCtrl) {

    m_chatBox.property_halign().set_value(Gtk::Align::ALIGN_START);
    m_chatBox.property_valign().set_value(Gtk::Align::ALIGN_END);

    m_chatBox.set_vexpand(true);
    m_chatBox.set_hexpand(true);
    m_chatBox.show();

    m_chatBox.attach(m_msgEntry, 0, 6, 5, 1);
    m_msgEntry.show();
    m_msgEntry.set_width_chars(50);
    m_msgEntry.signal_activate().connect(sigc::mem_fun(*this, &chat_view::chat_enter));
    m_msgEntry.property_placeholder_text().set_value("Type Message");

    m_chatBox.attach(m_msgWindow, 0, 0, 5, 5);
    m_msgWindow.show();
    m_msgWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
    m_msgWindow.set_size_request(500, 300);

    m_msgWindow.add(m_msgView);
    m_msgView.show();
    m_msgView.set_vexpand(true);
    m_msgView.set_hexpand(true);
    m_msgView.set_editable(false);
    m_msgView.set_cursor_visible(false);
    m_msgView.set_opacity(0.75);
    m_msgView.set_focus_on_click(false);
}

void user_interface::views::chat_view::open() {
    m_msgEntry.set_can_default();
    m_viewCtrl.window().set_can_default();
    m_viewCtrl.window().set_default(m_msgEntry);
}

void user_interface::views::chat_view::close() {

}

void user_interface::views::chat_view::event_callback(const types::game_events event) {
    if (event == types::game_events::recv_chat_message) {        
        chat_recv();
    }
}

Gtk::Overlay& user_interface::views::chat_view::container() {
    return m_emptyOverlay;
}

Gtk::Container& user_interface::views::chat_view::overlay() {
    return m_chatBox;
}

void user_interface::views::chat_view::chat_enter() {
    if (m_msgEntry.has_focus()) {
        if (m_msgEntry.get_text_length() > 0) {
            m_viewCtrl.config().global().set<std::string>("chat-message", m_msgEntry.get_text());
            m_viewCtrl.create_event(types::game_events::enter_chat_message);
        }
        
        m_viewCtrl.create_event(types::game_events::chat_close);

        m_msgEntry.set_text("");
    } else {
        m_msgEntry.set_can_focus(true);
        m_msgEntry.grab_focus();
    }
}

void user_interface::views::chat_view::chat_recv() {
    bool scroll = !m_msgView.place_cursor_onscreen();
    
    m_receivedMessages.push(m_viewCtrl.config().global().get<std::string>("last-chat-message").second);

    insert_all_messages();

    auto textBuffer = m_msgView.get_buffer();
    textBuffer->place_cursor(textBuffer->end());

    if (scroll) {
        scroll_down();
    }
}

void user_interface::views::chat_view::insert_all_messages() {
    auto textBuffer = m_msgView.get_buffer();

    while (!m_receivedMessages.empty()) {
        std::string prefix;

        if (textBuffer->get_char_count() > 0) {
            prefix = "\n";
        }

        textBuffer->insert(textBuffer->end(), prefix + m_receivedMessages.front());

        m_receivedMessages.pop();
    }
}

void user_interface::views::chat_view::scroll_down() {
    auto textBuffer = m_msgView.get_buffer();

    // Wait for Gui to Redraw with added line to buffer
    // what so ever, seems like a really bad design choice,
    // but gtk seems to want some hacking like this
    while (gtk_events_pending()) {
        gtk_main_iteration_do(false);
    }

    // Scoll to end of Buffer
    auto it = textBuffer->get_iter_at_line(textBuffer->get_line_count());
    m_msgView.scroll_to(it);
}
