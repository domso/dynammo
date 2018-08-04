#include "src/user_interface/views/base_view.h"

void user_interface::views::base_view::event_dispatcher(const types::game_events event, user_interface::views::base_view* view) {
    gdk_threads_add_idle(
        event_callback_wrapper,
        new std::pair<
                types::game_events,
                user_interface::views::base_view*
            > (event, view)
    );
}

int user_interface::views::base_view::event_callback_wrapper(void* arg) {
    std::unique_ptr<
        std::pair<
            types::game_events,
            user_interface::views::base_view*
        >
    >  eventArg(
        static_cast<
            std::pair<
                types::game_events,
                user_interface::views::base_view*
            >*
        >(arg)
    );
    
    eventArg->second->event_callback(eventArg->first);
    
    return 0;
}
