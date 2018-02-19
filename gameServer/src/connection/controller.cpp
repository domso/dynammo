#include "controller.h"

connection::controller::controller(const uint16_t port) {
    m_workerThread = std::thread(&worker::accept_new_connections, &m_worker, port);
}

connection::controller::~controller() {
    close();
}

void connection::controller::close() {
    m_worker.close();
    if (m_workerThread.joinable()) {
        m_workerThread.join();
    }
}

network::tcp_connection<network::ipv4_addr> connection::controller::get_new_connection(const int id) {
    return m_worker.get_connection(id);
}


