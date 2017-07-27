#ifndef service_m_maph
#define service_m_maph

#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

#include "network/udp_socket.h"

#define SERVICE_MAP_HEADER_RESPONSE_ERROR 0x01

#define SERVICE_MAP_HEADER_REQUEST_REGISTER 0x02
#define SERVICE_MAP_HEADER_REQUEST_UNREGISTER 0x04
#define SERVICE_MAP_HEADER_REQUEST_SHUTDOWN 0x08


class service_map {


    struct service_info {
        network::ipv6_addr service_address;
        int date;
        int uptime;
        int QoS;
    };

    struct base_header {
        char status;
        int key;
    };

    struct lookup_response {
        base_header header;
        service_info info;
    };

    struct m_registerrequest {
        base_header header;
        int uptime;
        int QoS;
    };

public:

    service_map();

    service_map(const service_map& that) = delete;

    ~service_map();

    void init();

    bool registerService(int key, service_map::service_info& info);

    bool unregisterService(int key);

    bool getService(int key, service_map::service_info& info);

    void exec_data();

    void exec_contr();


private:

    std::atomic<bool> m_isRunning;
    std::mutex m_mutex;
    std::unordered_map<int, service_info> m_map;
    network::udp_socket<network::ipv6_addr> m_datam_socket;
    network::udp_socket<network::ipv6_addr> controll_m_socket;
};






#endif

