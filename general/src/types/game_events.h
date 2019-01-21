#pragma once

namespace types {
    enum game_events {
        clear,
        
        key_up,
        key_down,
        key_right,
        key_left,
        
        move_up,
        move_down,
        move_right,
        move_left,
        attack,
        
        
        move_up_region,
        move_down_region,
        move_right_region,
        move_left_region,
        
        enter_region,
        leave_region,
        
        request_login,
        success_login,
        failure_login,
        
        request_auth,
        request_account_creation,
        success_account_creation,
        failure_account_creation,
        
        enter_chat_message,
        recv_chat_message,
        chat_close
    };
}
