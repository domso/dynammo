#ifndef gameServer_controller_game_map_controller_h
#define gameServer_controller_game_map_controller_h

#include <vector>
#include "src/util/wait_lock.h"
#include "src/data/map/map_instance.h"

namespace controller {
    namespace game {
        class map_controller {
        public:
            
            bool init(const data::map::id start, const data::map::id end);
            
            data::map::map_instance* load(const data::map::id mapID);      
            
        private:
            data::map::id m_start;
            data::map::id m_end;
                       
            std::vector<data::map::map_instance> m_maps;
        };
    }
}

#endif
