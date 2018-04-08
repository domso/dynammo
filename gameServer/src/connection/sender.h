#pragma once

#include <vector>

#include "network/tcp_connection.h"

namespace connection {
    class sender {
    public:
        template <typename T>
        static bool send(network::tcp_connection<network::ipv4_addr>& connection, typename T::content& obj) {
            uint8_t protocolID = T::id;
            uint16_t count = 1;
            bool result = true;
            
            result &= connection.send_data<uint8_t>(&protocolID, 1).first;
            result &= connection.send_data<uint16_t>(&count, 1).first;
            result &= connection.send_data<typename T::content>(&obj, 1).first;
            
            return result;
        }
        
        template <typename T>
        static bool send(network::tcp_connection<network::ipv4_addr>& connection, std::vector<typename T::content>& objs) {
            uint8_t protocolID = T::id;
            uint16_t count = objs.size();
            bool result = true;
            
            if (count > 0) {
                result &= connection.send_data<uint8_t>(&protocolID, 1).first;
                result &= connection.send_data<uint16_t>(&count, 1).first;
                result &= connection.send_data<typename T::content>(objs.data(), count).first;
            }
            
            return result;
        }
    };
}
