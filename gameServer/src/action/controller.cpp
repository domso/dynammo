#include "src/action/controller.h"

action::controller::controller(region::controller& regionCtrl) : m_regionCtrl(regionCtrl) {
}

util::optional_ref<region::dynamic_obj> action::controller::execute_action(const uint32_t regionID, const uint32_t objID, const types::game_events action) {
    auto region = m_regionCtrl.get_region(regionID);
    region::dynamic_obj* obj = region->get_dynamic_obj(objID);

    if (obj != nullptr) {
        switch (action) {
        case types::game_events::move_up:
            move_up(*obj);
            break;

        case types::game_events::move_left:
            move_left(*obj);
            break;

        case types::game_events::move_down:
            move_down(*obj);
            break;

        case types::game_events::move_right:
            move_right(*obj);
            break;

        default:
            break;
        }
    }

   
    return util::optional_ref<region::dynamic_obj>(*obj);
}
