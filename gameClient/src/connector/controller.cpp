#include "src/connector/controller.h"

connector::controller::controller(network::ipv4_addr& tcpDestAddr, network::ipv4_addr& udpDestAddr, region::controller& regCtrl, session::controller& sessionCtrl, util::event_controller<types::game_events>& eventCtrl)
    : m_udpDestAddr(udpDestAddr),
      m_tcpDestAddr(tcpDestAddr),
      m_msgCtrl(bufferSize),
      m_tcpRecv(bufferSize),
      m_dataContext(sessionCtrl, m_tcpRecv, regCtrl),
      m_messageContext(m_msgCtrl, regCtrl, sessionCtrl),      
      m_requester(udpDestAddr, eventCtrl, m_msgCtrl, regCtrl, sessionCtrl)
      {}

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
    
    return m_status;
}
