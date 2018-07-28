#include "src/connector/context.h"
#include "src/connector/controller.h"
#include "src/util/signals.h"
#include "src/util/worker.h"

int main() {   
    std::cout << "Open Server" << std::endl;
    
    //data layer
    region::controller regionCtrl;    
    
    //network layer
    session::controller sessionCtrl;
    connector::controller connCtrl(regionCtrl, sessionCtrl);    
    
    util::worker<region::controller> w1(regionCtrl);
    util::worker<session::controller> w2(sessionCtrl);
    util::worker<connector::controller> w3(connCtrl);
    
    util::signals::wait_for_sigint();
    std::cout << "Close Server" << std::endl;   
        
    return 0;
}
