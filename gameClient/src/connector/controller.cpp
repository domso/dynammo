#include "src/connector/controller.h"

connector::controller::controller(region::controller& regCtrl, session::controller& sessionCtrl, util::event_controller<types::game_events>& eventCtrl, util::config_file& config)
    : m_config(config),
      m_msgCtrl(bufferSize),
      m_tcpRecv(bufferSize),
      m_dataContext(sessionCtrl, m_tcpRecv, regCtrl),
      m_messageContext(m_msgCtrl, regCtrl, sessionCtrl),      
      m_requester(m_udpDestAddr, eventCtrl, m_msgCtrl)
      {
          eventCtrl.register_event_handler<open_event>(this);
    }

connector::controller::~controller() {
    m_requester.close();
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
    bool status = true;
    status &= load_config();
    status &= m_msgCtrl.init(0) && m_tcpRecv.init(m_tcpDestAddr);

    if (status) {
        m_msgCtrlThread = std::thread(&message::msg_controller::recv, &m_msgCtrl);
        m_tcpRecvThread = std::thread(&connector::tcp_receiver::recv, &m_tcpRecv);
    }
    
    m_requester.open();

    return status;
}

types::game_events connector::controller::open_event::handle(const types::game_events event, connector::controller* ctrl) {
    ctrl->open();
    return types::game_events::clear;
}

bool connector::controller::load_config() {
    auto serverIP = m_config.get<std::string>("serverIP");
    auto tcpPort = m_config.get<uint16_t>("tcpPort");
    auto udpPort = m_config.get<uint16_t>("udpPort");
    
    if (serverIP.first && tcpPort.first && udpPort.first) {
        return m_tcpDestAddr.init(serverIP.second, tcpPort.second) && m_udpDestAddr.init(serverIP.second, udpPort.second);
    }   
    
    return false;
}

