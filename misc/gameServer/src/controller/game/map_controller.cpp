#include "src/controller/game/map_controller.h"

namespace controller {
    namespace game {
        bool map_controller::init(const data::map::id start, const data::map::id end) {
            m_start = start;
            m_end = end;

            m_maps.resize(end - start + 1);

            for (int i = start; i <= end; i++) {
                if (!m_maps[i].init(i)) {
                    for (int j = i; j >= start; j--) {
                        m_maps[j].close();
                    }

                    return false;
                }
            }

            return true;
        }

        data::map::map_instance* map_controller::load(const data::map::id mapID) {
            if (mapID < m_maps.size()) {
                return &m_maps[mapID];
            }

            return nullptr;
        }
    }
}
