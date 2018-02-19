#include "src/connector/controller.h"

connector::controller::controller(network::ipv4_addr& tcpDestAddr, network::ipv4_addr& udpDestAddr)
    : m_outputBuffer(bufferSize),
      m_udpDestAddr(udpDestAddr),
      m_tcpDestAddr(tcpDestAddr),
      m_msgCtrl(bufferSize),
      m_tcpRecv(bufferSize) {}

connector::controller::~controller() {
    m_msgCtrl.close();

    if (m_msgCtrlThread.joinable()) {
        m_msgCtrlThread.join();
    }

    m_tcpRecv.close();

    if (m_tcpRecvThread.joinable()) {
        m_tcpRecvThread.join();
    }
}

bool connector::controller::open() {
    m_status = m_msgCtrl.init(0) && m_tcpRecv.init(m_tcpDestAddr);

    if (m_status) {
        m_msgCtrlThread = std::thread(&message::msg_controller::recv, &m_msgCtrl);
        m_tcpRecvThread = std::thread(&connector::tcp_receiver::recv, &m_tcpRecv);
    }
}
