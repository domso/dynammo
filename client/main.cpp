#include "src/config/controller.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"

#include "src/user_interface/controller.h"
#include "src/game/controller.h"

#include "src/connector/controller.h"
#include "src/connector/data_transfer/controller.h"

#include "src/util/worker.h"

int main(int argc, char* argv[]) {    
    // global-layer
    config::controller config;
    util::event_controller<types::game_events> eventCtrl;
 
    // graphic-layer
    user_interface::controller interfaceCtrl(argc, argv, eventCtrl, config);
    game::controller gameCtrl(interfaceCtrl, eventCtrl, config);
        
    // network-layer    
    connector::context connectorContext(gameCtrl, eventCtrl, config);
    connector::controller connectorCtrl(connectorContext);
    connector::data_transfer::controller dataCtrl(connectorContext);
    
    util::worker w1(connectorCtrl);
    util::worker w2(dataCtrl);
    util::worker w3(interfaceCtrl);
    
    w3.wait_for_close();
    return 0;
}
