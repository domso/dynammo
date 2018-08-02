#include "src/connector/data_transfer/controller.h"
#include <cassert>

connector::data_transfer::controller::controller(connector::context& context)
    : m_context(context),
      m_tcpRecv(buffersize)
{
    register_link(sessionIDLink);
    register_link(regionSelectLink);
    register_link(layerLink);
    register_link(staticObjLink);
    register_link(dynObjLink);
    register_link(regionChatLink);
}

void connector::data_transfer::controller::update() {
    auto serverIP = m_context.config.global().get<std::string>("serverIP", "127.0.0.1");
    auto tcpPort = m_context.config.global().get<uint16_t>("tcpPort", 1850);
    
    network::ipv4_addr destAddr;
    assert(destAddr.init(serverIP.second, tcpPort.second));
    assert(m_tcpRecv.init(destAddr));
    m_tcpRecv.recv();    
}

void connector::data_transfer::controller::close() {
    m_tcpRecv.close();
}
