#include "src/controller/game/accept_controller.h"

namespace controller {
    namespace game {
        accept_controller::accept_controller(const options_t& options) : m_currentOptions(options), m_pendingConnections(options.maxPending) {

        }

        accept_controller::~accept_controller() {
            if (m_mainThreadHandle.joinable()) {
                m_mainThreadHandle.join();
            }
        }

        bool accept_controller::start() {
            bool current = false;

            if (m_isRunning.compare_exchange_strong(current, true)) {
                if (!m_socket.accept_on(m_currentOptions.port, m_currentOptions.backlog)) {
                    return false;
                }

                m_socket.set_timeout(m_currentOptions.updateIntervall);

                if (m_mainThreadHandle.joinable()) {
                    m_mainThreadHandle.join();
                }

                m_mainThreadHandle = std::thread(&accept_controller::main_thread, this);

                return true;
            }

            return false;
        }

        bool accept_controller::stop() {
            bool current = true;
            return m_isRunning.compare_exchange_strong(current, false);
        }

        void accept_controller::main_thread() {
            int currentIndex = 0;
            authentication::ticket_t currentTicket = 0;

            while (m_isRunning.load()) {
                auto connection = m_socket.accept_connection();
                local_connection_context_t& slot = m_pendingConnections[currentIndex];

                if (connection != nullptr) {
                    std::lock_guard<std::mutex>(slot.mutex);
                    slot.ticket = currentTicket;
                    slot.connection = std::move(connection);
                    slot.connection->send_data<authentication::ticket_t>(&currentTicket, 1);
                }

                currentTicket++;
                currentIndex = (currentIndex + 1) % m_pendingConnections.size();
            }
        }

        std::shared_ptr<network::tcp_connection<network::ipv4_addr>> accept_controller::get_connection(const authentication::ticket_t ticket) {
            std::shared_ptr<network::tcp_connection<network::ipv4_addr>> emptyResult = nullptr;

            for (auto& c : m_pendingConnections) {
                if (c.ticket.load(std::memory_order_acquire) == ticket) {
                    std::lock_guard<std::mutex>(c.mutex);

                    if (c.ticket.load(std::memory_order_acquire) == ticket) {
                        return std::move(c.connection);
                    }
                }
            }

            return emptyResult;
        }
    }
}

