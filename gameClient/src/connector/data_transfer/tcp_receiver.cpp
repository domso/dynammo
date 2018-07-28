#include "src/connector/data_transfer/tcp_receiver.h"

#include "src/util/mem.h"

connector::data_transfer::tcp_receiver::tcp_receiver(const int bufferSize) : m_currentMode(0), m_modeIsValid(false), m_inputBuffer(bufferSize) {
    for (int i = 0; i < 256; i++) {
        m_configureCallback[i] = nullptr;
        m_completeCallback[i] = nullptr;
        m_callbackDataArg[i] = nullptr;
    }
}

connector::data_transfer::tcp_receiver::~tcp_receiver() {
    close();
}

bool connector::data_transfer::tcp_receiver::init(const network::ipv4_addr& addr) {
    if (m_connection.connect_to(addr)) {
        m_connection.set_timeout(0.5);

        return true;
    }

    return false;
}

void connector::data_transfer::tcp_receiver::close() {
    if (!m_connection.is_closed()) {
        m_connection.close_socket();
    }
}

void connector::data_transfer::tcp_receiver::recv() {
    while (internal_recv()) {
        std::unique_lock<std::mutex> ul(m_mutex);
        call_configure(ul);
        copy_bytes();
        call_complete(ul);
    }
}

bool connector::data_transfer::tcp_receiver::internal_recv() {
    if (m_inputBuffer.remaining_msg_length() != 0) {
        return true;
    }

    return m_connection.recv_pkt(m_inputBuffer).first;
}

void connector::data_transfer::tcp_receiver::call_configure(std::unique_lock<std::mutex>& ul) {
    if (!m_modeIsValid && m_inputBuffer.msg_length() > 0) {        
        auto newMode = m_inputBuffer.get_next<uint8_t>();
        m_currentMode = *newMode;
        m_modeIsValid = m_configureCallback[m_currentMode] != nullptr;
    }

    if (m_currentTarget.complete() && m_modeIsValid && m_configureCallback[m_currentMode] != nullptr) {
        ul.unlock();
        m_currentTarget = m_configureCallback[m_currentMode](m_callbackDataArg[m_currentMode]);
        ul.lock();
    }
}

void connector::data_transfer::tcp_receiver::call_complete(std::unique_lock<std::mutex>& ul) {
    if (m_currentTarget.complete() && m_modeIsValid && m_completeCallback[m_currentMode] != nullptr) {
        ul.unlock();
        m_modeIsValid = !m_completeCallback[m_currentMode](m_callbackDataArg[m_currentMode]);
        ul.lock();
    }
}

void connector::data_transfer::tcp_receiver::copy_bytes() {
    int recvBytes = m_inputBuffer.remaining_msg_length();
    int minSize = std::min(recvBytes, m_currentTarget.size());
    util::mem::cpy<uint8_t>(m_currentTarget.destination(), m_inputBuffer.get_next<uint8_t>(minSize), minSize);    
    m_currentTarget.set<uint8_t>(m_currentTarget.destination() + minSize, m_currentTarget.size() - minSize);
}

connector::data_transfer::tcp_receiver::target::target()                                            : m_destPtr(nullptr), m_destSize(0) {}
connector::data_transfer::tcp_receiver::target::target(const connector::tcp_receiver::target& copy) : m_destPtr(copy.m_destPtr), m_destSize(copy.m_destSize) {}
connector::data_transfer::tcp_receiver::target::target(connector::tcp_receiver::target&& move)      : m_destPtr(move.m_destPtr), m_destSize(move.m_destSize) {move.reset();}

void connector::data_transfer::tcp_receiver::target::operator=(const connector::tcp_receiver::target& copy) {
    m_destPtr = copy.m_destPtr;
    m_destSize = copy.m_destSize;
}

void connector::data_transfer::tcp_receiver::target::operator=(connector::tcp_receiver::target && move) {
    m_destPtr = move.m_destPtr;
    m_destSize = move.m_destSize;    
    
    move.m_destPtr = nullptr;
    move.m_destSize = 0;
}

uint8_t* connector::data_transfer::tcp_receiver::target::destination() {
    return m_destPtr;
}

int connector::data_transfer::tcp_receiver::target::size() const {
    return m_destSize;
}

bool connector::data_transfer::tcp_receiver::target::complete() const {
    return m_destSize == 0;
}

void connector::data_transfer::tcp_receiver::target::reset() {
    m_destPtr = nullptr;
    m_destSize = 0;
}
