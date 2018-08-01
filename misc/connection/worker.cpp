#include "src/connection/worker.h"

#include "src/connection/sender.h"
#include "src/types/data_transfer/content.h"

connection::worker::worker() : m_error(false), m_running(true), m_mutexes(max_number_of_pending_connections), m_connections(max_number_of_pending_connections) {
    
}

connection::worker::~worker() {
    close();
}

void connection::worker::accept_new_connections(const uint16_t port) {    
    network::tcp_socket<network::ipv4_addr> tcpSocket;
    int32_t position = 0;
    
    if (tcpSocket.accept_on(port, 10)) {        
        tcpSocket.set_timeout(1);
        while (m_running) {
            std::lock_guard<std::mutex> lg(m_mutexes[position]);
            if (tcpSocket.accept_connection(m_connections[position])) {                
                sender::send<types::data_transfer::content::auth_ticket>(m_connections[position], position);
                
                position = ((position + 1) % max_number_of_pending_connections);
            }
        }
    }
    
    m_error = true;
}

network::tcp_connection<network::ipv4_addr> connection::worker::get_connection(const int i) {
    if (0 <= i && i < max_number_of_pending_connections) {        
        std::unique_lock<std::mutex> ul(m_mutexes[i], std::try_to_lock);
        if (ul.owns_lock()) {
            return std::move(m_connections[i]);
        }
    }
    
    return network::tcp_connection<network::ipv4_addr>();
}

bool connection::worker::error() const {
    return m_error;
}

void connection::worker::close() {
    m_running.store(false);
}