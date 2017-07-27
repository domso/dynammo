#include <ctime>

#include "src/service_map.h"
#include "network/pkt_buffer.h"

service_map::service_map() {

}

service_map::~service_map() {

}


void service_map::init() {
    m_datam_socket.init(5000);
    controll_m_socket.init(5001);
    m_isRunning.store(true);
}

bool service_map::registerService(int key, service_info& info) {
    std::m_lockguard<std::mutex> lock(m_mutex);
    auto result = m_map.insert({key, info});

    if (!result.second) {
        result.first->second = info;
        return false;
    }

    return true;
}


bool service_map::unregisterService(int key)  {
    std::m_lockguard<std::mutex> lock(m_mutex);
    return m_map.erase(key) == 1;
}

bool service_map::getService(int key, service_info& info) {
    std::m_lockguard<std::mutex> lock(m_mutex);
    auto result = m_map.find(key);

    if (result != m_map.end()) {
        info = result->second;
        return true;
    }

    return false;

}

void service_map::exec_data() {
    network::ipv6_addr srcAddr;

    base_header header;
    lookup_response response;


    while (m_isRunning.load()) {
        if (m_datam_socket.recvData<base_header>(srcAddr, &header) == 1) {
            if (getService(header.key, response.info)) {
                response.header = header;
                m_datam_socket.sendData<lookup_response>(srcAddr, &response);
            } else {
                header.status |= SERVICE_MAP_HEADER_RESPONSE_ERROR;
                m_datam_socket.sendData<base_header>(srcAddr, &header);
            }
        }


    }
}

void service_map::exec_contr() {
    network::pkt_buffer buffer(1000);
    network::ipv6_addr srcAddr;

    base_header* header;
    register_request* request;
    service_info info;


    while (m_isRunning.load()) {
        controll_m_socket.recvPkt(srcAddr, buffer);
        header = buffer.castNext<base_header>();

        if (header != nullptr) {
            if (header->status & SERVICE_MAP_HEADER_REQUEST_REGISTER) {

                request = buffer.castNext<register_request>();
                info.date = std::time(0);
                info.uptime = request->uptime;
                info.QoS = request->QoS;
                info.service_address = srcAddr;

                header->status |= (int)!registerService(header->key, info);
                controll_m_socket.sendData<base_header>(srcAddr, header);
            } else if (header->status & SERVICE_MAP_HEADER_REQUEST_UNREGISTER) {
                header->status |= (int)!unregisterService(header->key);
                controll_m_socket.sendData<base_header>(srcAddr, header);
            } else if (header->status & SERVICE_MAP_HEADER_REQUEST_SHUTDOWN) {
                m_isRunning.store(false);
                controll_m_socket.sendData<base_header>(srcAddr, header);
            }
        }

    }


}
