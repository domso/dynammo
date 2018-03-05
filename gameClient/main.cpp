
#include "src/connector/controller.h"

#include "src/region/controller.h"
#include "src/graphic/controller.h"

#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/session/controller.h"


int main(int argc, char* argv[]) {  
    util::event_controller<types::game_events> eventCtrl(types::game_events::clear);
       
    network::ipv4_addr tcpAddr;
    tcpAddr.init("127.0.0.1", 1850);

    network::ipv4_addr udpAddr;
    udpAddr.init("127.0.0.1", 1851);
    
    // graphic-layer
    graphic::controller graphicCtrl(argc, argv, eventCtrl);
    graphicCtrl.open();
        
    // data-layer
    region::controller regionCtrl(graphicCtrl);
    session::controller sessionCtrl(eventCtrl);
    
    // network-layer
    connector::controller connectorCtrl(tcpAddr, udpAddr, regionCtrl, sessionCtrl, eventCtrl);   
    connectorCtrl.open();

    graphicCtrl.wait_for_close();

    return 0;
}
