#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>

#include "network/pkt_buffer.h"
#include "network/tcp_connection.h"
#include "src/util/lock_ref.h"
#include "src/authentication/types.h"
#include "src/encryption/public_key.h"

namespace session {
    class user : public util::locked_ref_item {
    public:
        user();
        user(const user& copy) = delete;
        user(user&& move) = delete;
        ~user();

        bool set_accountID(const authentication::accountID_t accountID);
        uint32_t get_accountID() const;

        void set_connection(network::tcp_connection<network::ipv4_addr>& conn);
        bool is_connected() const;

        void set_addr(const network::ipv4_addr& addr);
        const network::ipv4_addr& get_addr() const;
        bool verify_buffer(network::pkt_buffer& inputBuffer);

        template <typename T>
        void send(const typename T::content& obj) {
            uint8_t protocolID = T::id;
            uint16_t count = 1;
            bool result = true;

            result &= m_connection.send_data<uint8_t>(&protocolID, 1).first;
            result &= m_connection.send_data<uint16_t>(&count, 1).first;
            result &= m_connection.send_data<typename T::content>(&obj, 1).first;

            m_connectionState = result;
        }

        template <typename T>
        void send(const std::vector<typename T::content>& objs) {
            uint8_t protocolID = T::id;
            uint16_t count = objs.size();
            bool result = true;

            if (count > 0) {
                result &= m_connection.send_data<uint8_t>(&protocolID, 1).first;
                result &= m_connection.send_data<uint16_t>(&count, 1).first;
                result &= m_connection.send_data<typename T::content>(objs.data(), count).first;
            }

            m_connectionState = result;
        }

        template <typename T, typename keyT>
        void send(const std::unordered_map<keyT, typename T::content>& objs) {
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

                m_connectionState = m_connection.send_pkt(buffer).first;
            }

            m_connectionState = false;
        }
    private:
        network::tcp_connection<network::ipv4_addr> m_connection;
        network::ipv4_addr m_udpAddr;
        encryption::public_key m_publicKey;
        uint32_t m_accountID;
        bool m_connectionState;
    };
}
