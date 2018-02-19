
#include "src/message/msg_controller.h"
#include "src/message/content.h"
#include "src/message/info.h"
#include "src/message/auth.h"

#include "src/region/context.h"

#include <iostream>
#include <thread>

#include "src/data/map.h"

#include "src/util/obj_cache.h"

#include "src/util/file_storage.h"
#include "network/tcp_connection.h"
#include "src/message/msg_controller.h"
#include "src/connector/tcp_receiver.h"

#include "src/connector/controller.h"

#include "src/data/context.h"
#include "src/message/context.h"

int main(int argc, char* argv[]) {
    network::ipv4_addr tcpAddr;
    tcpAddr.init("127.0.0.1", 1850);
    
    network::ipv4_addr udpAddr;
    udpAddr.init("127.0.0.1", 1851);
       
    connector::controller c(tcpAddr, udpAddr);
    data::context dataContext(c);
    message::context messageContext(c);
    
    c.open();
     
    
    dataContext.regionData.currentState.wait_for(message::types::states::recv);
    
    auto app = Gtk::Application::create(argc, argv);

    Example_GLArea test;
    
    data::map m(dataContext.regionData.region0);
    m.build();

    test.add_mesh(&m.m_mesh);
    
    
    //Shows the window and returns when it is closed.
    return app->run(test);
    
    return 0;
}
