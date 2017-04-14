#ifndef gameServer_data_map_h
#define gameServer_data_map_h

#include <vector>
#include "include/data/world_tile.h"

namespace data {
    
    typedef uint32_t mapID_t;
    
    class map {
        
        std::vector<data::world_tile_t> tiles_;
    };
    
        
    struct map_handle_t {
        data::map* map;
        mapID_t mapID;
    };
}

#endif