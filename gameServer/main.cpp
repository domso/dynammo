

#include "src/connector/context.h"
#include "src/connector/worker.h"
#include "src/util/signals.h"



int main() {   
    std::cout << "Open Server" << std::endl;
    
    //data layer
    region::controller regionCtrl;
    action::controller actionCtrl(regionCtrl);
    user::controller userCtrl;
    
    //network layer
    connection::controller connectionCtrl(1850);
    connector::context c1(regionCtrl, actionCtrl, userCtrl, connectionCtrl);
    connector::worker w1(c1);
    connector::worker w2(c1);
    w1.init_as(connector::worker::receiver);
//     w2.init_as(connector::worker::updater);
    
    util::signals::wait_for_sigint();
    std::cout << "Close Server" << std::endl;   
        
    return 0;
}
