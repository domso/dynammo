#include "src/user_interface/view_controller.h"

user_interface::view_controller::view_controller(Gtk::Box& rootBox) : m_rootBox(rootBox) {

}

void user_interface::view_controller::change_view(const view_list::views newView) {
    close_current_view();
    m_currentView = newView;
    open_current_view();
}

void user_interface::view_controller::open_current_view() {
    auto result = m_views.find(m_currentView.get());

    if (result != m_views.end()) {
        auto& view = *result->second;
        m_rootBox.add(view.container());
        view.open();
        view.container().show();
    }
}

void user_interface::view_controller::close_current_view() {
    auto result = m_views.find(m_currentView.get());

    if (result != m_views.end()) {
        auto& view = *result->second;
        view.close();
        view.container().hide();
        m_rootBox.remove(view.container());
    }
}
