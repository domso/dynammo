
#include "src/connector/controller.h"

#include "src/data/context.h"
#include "src/message/context.h"

#include "src/region/controller.h"
#include "src/graphic/controller.h"

int main(int argc, char* argv[]) {  
    network::ipv4_addr tcpAddr;
    tcpAddr.init("127.0.0.1", 1850);

    network::ipv4_addr udpAddr;
    udpAddr.init("127.0.0.1", 1851);
    
    // graphic-layer
    graphic::controller graphicCtrl(argc, argv);
    graphicCtrl.open();
        
    // data-layer
    region::controller regionCtrl(graphicCtrl);
    
    // network-layer
    connector::controller connectorCtrl(tcpAddr, udpAddr);   
    data::context dataContext(connectorCtrl, regionCtrl);
    message::context messageContext(connectorCtrl, regionCtrl);
    connectorCtrl.open();

    graphicCtrl.wait_for_close();

    return 0;
}
