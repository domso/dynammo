#include "src/user_interface/views/code_view.h"

#include "src/assembler.h"
#include <iomanip>

user_interface::views::code_view::code_view(
    view_controller& viewCtrl,
    graphic::renderer& renderer,
    key_controller& keyCtrl
) : m_viewCtrl(viewCtrl), m_keyCtrl(keyCtrl) {

    m_chatBox.property_halign().set_value(Gtk::Align::ALIGN_CENTER);
    m_chatBox.property_valign().set_value(Gtk::Align::ALIGN_CENTER);

    m_chatBox.set_vexpand(true);
    m_chatBox.set_hexpand(true);
    m_chatBox.show();

    m_chatBox.attach(m_acceptButton, 5, 0, 1, 5);
    m_acceptButton.show();
    m_acceptButton.set_label("-->");
    m_acceptButton.signal_clicked().connect(sigc::mem_fun(*this, &code_view::assemble));
    
    m_chatBox.attach(m_assemblyWindow, 0, 0, 5, 5);
    m_assemblyWindow.show();
    m_assemblyWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
    m_assemblyWindow.set_size_request(800, 500);

    m_assemblyWindow.add(m_assembly);
    m_assembly.show();
    m_assembly.set_vexpand(true);
    m_assembly.set_hexpand(true);
    m_assembly.set_editable(true);
    m_assembly.set_cursor_visible(true);
    m_assembly.set_opacity(0.75);
    m_assembly.set_focus_on_click(true);
    
    m_chatBox.attach(m_binaryWindow, 6, 0, 5, 5);
    m_binaryWindow.show();
    m_binaryWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
    m_binaryWindow.set_size_request(800, 500);

    m_binaryWindow.add(m_binary);
    m_binary.show();
    m_binary.set_vexpand(true);
    m_binary.set_hexpand(true);
    m_binary.set_editable(false);
    m_binary.set_cursor_visible(false);
    m_binary.set_opacity(0.75);
    m_binary.set_focus_on_click(false);   
    
}

void user_interface::views::code_view::open() {

}

void user_interface::views::code_view::close() {

}

void user_interface::views::code_view::event_callback(const types::game_events event) {

}

Gtk::Overlay& user_interface::views::code_view::container() {
    return m_emptyOverlay;
}

Gtk::Container& user_interface::views::code_view::overlay() {
    return m_chatBox;
}

void user_interface::views::code_view::assemble() {
    auto buffer = m_assembly.get_buffer();
    std::string text = buffer->get_text();
    riscv_asm::assembler converter;
    
    if (text.length() > 0) {
        m_currentCode = converter.assemble(text);
        std::stringstream stream;
        for (auto i : m_currentCode) {
            stream << std::hex << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << i << "\n";            
        }
        m_binary.get_buffer()->set_text(stream.str());
    }
}
