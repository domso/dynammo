#pragma once

#include <vector>
#include <unordered_map>

#include "network/tcp_connection.h"

namespace connection {
    class sender {
    public:
        template <typename T>
        static bool send(network::tcp_connection<network::ipv4_addr>& connection, const typename T::content& obj) {
            uint8_t protocolID = T::id;
            uint16_t count = 1;
            bool result = true;

            result &= connection.send_data<uint8_t>(&protocolID, 1).first;
            result &= connection.send_data<uint16_t>(&count, 1).first;
            result &= connection.send_data<typename T::content>(&obj, 1).first;

            return result;
        }

        template <typename T>
        static bool send(network::tcp_connection<network::ipv4_addr>& connection, const std::vector<typename T::content>& objs) {
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

        template <typename T, typename keyT>
        static bool send(network::tcp_connection<network::ipv4_addr>& connection, const std::unordered_map<keyT, typename T::content>& objs) {
            uint8_t protocolID = T::id;
            uint16_t count = objs.size();
            network::pkt_buffer buffer(1024);

            if (count > 0) {
                auto p = buffer.push_next<uint8_t>();

                if (p != nullptr) {
                    *p = protocolID;
                }

                auto c = buffer.push_next<uint16_t>();

                if (c != nullptr) {
                    *c = count;
                }

                for (auto& it : objs) {
                    auto o = buffer.push_next<typename T::content>();
                    *o = it.second;
                }
                
                return connection.send_pkt(buffer).first;
            }

            return false;
        }
    };
}
