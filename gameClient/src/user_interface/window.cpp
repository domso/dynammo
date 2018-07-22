#include "src/user_interface/window.h"

#include "src/user_interface/views/start_view.h"
#include "src/user_interface/views/main_game_view.h"
#include "src/user_interface/views/create_account_view.h"

user_interface::window::window(graphic::renderer& renderer, util::event_controller<types::game_events>& eventCtrl, util::config_file& config) :
    m_keyCtrl(eventCtrl),
    m_renderer(renderer),
    m_viewCtrl(m_rootBox, eventCtrl, config)
//     m_startView(m_viewCtrl),
//     m_mainGameView(m_viewCtrl, renderer, m_keyCtrl)
{
    m_viewCtrl.register_view<views::start_view>();
    m_viewCtrl.register_view<views::create_account_view>();
    m_viewCtrl.register_view<views::main_game_view>(std::ref(m_renderer), std::ref(m_keyCtrl));
    
    
    
    
    set_title("GL Area");
    set_default_size(800, 600);
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::SCROLL_MASK);
//     m_VBox.property_margin() = 12;

    add(m_rootBox);
    m_rootBox.show();


    signal_key_press_event().connect(sigc::mem_fun(*this, &window::key_press_event), true);
    signal_key_release_event().connect(sigc::mem_fun(*this, &window::key_release_event), true);
}

user_interface::window::~window() {

}

bool user_interface::window::key_press_event(GdkEventKey* event) {
    //TODO work around for moving the camera
    if (event->keyval == 'W') {
        m_renderer.unpublishedSettings.cameraY += 0.1;
        m_renderer.publish_settings();
    }

    if (event->keyval == 'A') {
        m_renderer.unpublishedSettings.cameraX -= 0.1;
        m_renderer.publish_settings();
    }

    if (event->keyval == 'S') {
        m_renderer.unpublishedSettings.cameraY -= 0.1;
        m_renderer.publish_settings();
    }

    if (event->keyval == 'D') {
        m_renderer.unpublishedSettings.cameraX += 0.1;
        m_renderer.publish_settings();
    }



    m_keyCtrl.key_was_pressed(event->keyval);
    return false;
}

bool user_interface::window::key_release_event(GdkEventKey* event) {
    m_keyCtrl.key_was_released(event->keyval);
    return false;
}

bool user_interface::window::on_scroll_event(GdkEventScroll* scroll_event) {
    double dX;
    double dY;

    if (scroll_event->direction == GDK_SCROLL_SMOOTH) {
        gdk_event_get_scroll_deltas((GdkEvent*)scroll_event, &dX, &dY);
    } else {
        dX = scroll_event->delta_x;
        dY = scroll_event->delta_y;
    }

    m_renderer.unpublishedSettings.zoomX -= m_renderer.unpublishedSettings.scrollSpeed * dY;
    m_renderer.unpublishedSettings.zoomY -= m_renderer.unpublishedSettings.scrollSpeed * dY;
    m_renderer.publish_settings();

    return false;
}

bool user_interface::window::on_delete_event(GdkEventAny* event) {
    m_renderer.close();
    return false;
}

std::string user_interface::window::get_option(const std::string& key) {
    return m_viewCtrl.get_option(key);
}
