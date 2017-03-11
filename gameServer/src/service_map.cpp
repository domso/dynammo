#include <ctime>

#include "include/service_map.h"
#include "network/pkt_buffer.h"

service_map::service_map() {

}

service_map::~service_map() {

}


void service_map::init() {
    data_socket_.init(5000);
    controll_socket_.init(5001);
    isRunning_.store(true);
}

bool service_map::registerService(int key, service_info& info) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto result = map_.insert( {key, info});

    if (!result.second) {
        result.first->second = info;
        return false;
    }

    return true;
}


bool service_map::unregisterService(int key)  {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.erase(key) == 1;
}

bool service_map::getService(int key, service_info& info) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto result = map_.find(key);

    if (result != map_.end()) {
        info = result->second;
        return true;
    }

    return false;

}

void service_map::exec_data() {
    network::ipv6_addr srcAddr;

    base_header header;
    lookup_response response;


    while (isRunning_.load()) {
        if (data_socket_.recvData<base_header>(srcAddr, &header) == 1) {
            if (getService(header.key, response.info)) {
                response.header = header;
                data_socket_.sendData<lookup_response>(srcAddr, &response);
            } else {
                header.status |= SERVICE_MAP_HEADER_RESPONSE_ERROR;
                data_socket_.sendData<base_header>(srcAddr, &header);
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


    while (isRunning_.load()) {
        controll_socket_.recvPkt(srcAddr, buffer);
        header = buffer.castNext<base_header>();

        if (header != nullptr) {
            if (header->status & SERVICE_MAP_HEADER_REQUEST_REGISTER) {

                request = buffer.castNext<register_request>();
                info.date = std::time(0);
                info.uptime = request->uptime;
                info.QoS = request->QoS;
                info.service_address = srcAddr;

                header->status |= (int)!registerService(header->key, info);
                controll_socket_.sendData<base_header>(srcAddr, header);
            } else if (header->status & SERVICE_MAP_HEADER_REQUEST_UNREGISTER) {
                header->status |= (int)!unregisterService(header->key);
                controll_socket_.sendData<base_header>(srcAddr, header);
            } else if (header->status & SERVICE_MAP_HEADER_REQUEST_SHUTDOWN) {
                isRunning_.store(false);
                controll_socket_.sendData<base_header>(srcAddr, header);
            }
        }

    }


}
