
#include <iostream>


#include "network/tcp_connection.h"
#include "network/udp_socket.h"
#include "include/world.h"

#include "include/config_file_loader.h"
#include "include/service_map.h"

#include "include/util.h"

void controllThread(service_map* sm) {
    sm->exec_contr();
}


int main(int argc, char** argv) {

    std::cout << sizeof(network::tcp_connection<network::ipv4_addr>) << std::endl;















    return 0;

    config_file f;

    f.insert(" xX x : asd0x23  ");

    return 0;

    service_map sm;

    sm.init();

    std::thread t1(controllThread, &sm);
    sm.exec_data();

    t1.join();
    return 0;
}

