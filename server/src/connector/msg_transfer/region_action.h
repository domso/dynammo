#pragma once

#include "network/udp_socket.h"
#include "src/types/data_transfer/content.h"
#include "src/message/msg_header.h"
#include "src/types/msg_transfer/content.h"
#include "src/connector/context.h"

namespace connector {
    namespace msg_transfer {
        class region_action {
        public:
            typedef ::types::msg_transfer::content::region_action content;
            constexpr static const auto id = content::id;
            
            static bool request(
                message::msg_header_t& header,
                network::ipv4_addr& destAddr,
                network::pkt_buffer& outputBuffer,
                network::udp_socket<network::ipv4_addr>& socket,
                std::pair<uint32_t, std::vector<region::dynamic_obj>&>* data,
                connector::context* context
            );
            
            static message::msg_status_t requestHandler(
                message::msg_header_t& header,
                network::ipv4_addr& srcAddr,
                network::pkt_buffer& inputBuffer,
                network::pkt_buffer& outputBuffer,
                network::udp_socket<network::ipv4_addr>& socket,
                message::msg_option_t& options,
                connector::context* context
            );
            static message::msg_status_t responseHandler(
                message::msg_header_t& header,
                network::ipv4_addr& srcAddr,
                network::pkt_buffer& inputBuffer,
                network::pkt_buffer& outputBuffer,
                network::udp_socket<network::ipv4_addr>& socket,
                message::msg_option_t& options,
                connector::context* context
            );
        private:
            static void udp_broadcast(
                const network::pkt_buffer& outputBuffer,
                network::udp_socket<network::ipv4_addr>& socket,
                const std::vector<uint32_t>& users,
                session::controller& sessionCtrl
            );            
            static std::vector<uint32_t> tcp_broadcast(
                region::context& region,
                const std::vector<region::static_obj>& statics,
                const std::vector<region::dynamic_obj>& dynamics,
                const std::vector<region::layer<uint32_t>>& layers,
                const std::vector<uint32_t>& users,
                session::controller& sessionCtrl
            );                  
            static void send_tcp(
                region::context& region,
                session::controller& sessionCtrl
            );            
            static void send_udp(
                region::context& region,
                network::pkt_buffer& outputBuffer,
                network::udp_socket<network::ipv4_addr>& socket,
                session::controller& sessionCtrl
            );            
            static message::msg_status_t verify_request(
                content::types::request* request,
                message::msg_header_t& header,
                network::ipv4_addr& srcAddr,
                network::pkt_buffer& inputBuffer,
                network::pkt_buffer& outputBuffer,
                network::udp_socket<network::ipv4_addr>& socket,
                message::msg_option_t& options,
                connector::context* context
            );           
        };
    }
}

