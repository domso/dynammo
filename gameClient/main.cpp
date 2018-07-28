
#include "src/connector/controller.h"

#include "src/region/controller.h"
#include "src/graphic/controller.h"

#include "src/util/event_controller.h"
#include "src/types/game_events.h"
#include "src/util/config_file.h"

int main(int argc, char* argv[]) {    
    // global-layer
    util::config_file config;
    util::event_controller<types::game_events> eventCtrl;
 
    // graphic-layer
    graphic::controller graphicCtrl(argc, argv, eventCtrl, config);
    graphicCtrl.open();
    
    // data-layer
    region::controller regionCtrl;
    
    // network-layer    
    connector::controller connectorCtrl(regionCtrl, graphicCtrl, eventCtrl, config);   
    
    graphicCtrl.wait_for_close();
    return 0;
}
