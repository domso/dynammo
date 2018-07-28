#include "src/game/region_mesh_controller.h"

game::region_mesh_controller::region_mesh_controller(
    const uint32_t regionID,
    graphic::renderer& renderer,
    graphic::texture_controller& texCtrl
) : 
    m_regionID(regionID),
    m_renderer(renderer),
    m_texCtrl(texCtrl) 
    {
}


