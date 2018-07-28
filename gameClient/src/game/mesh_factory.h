#pragma once

#include "src/graphic/renderer.h"
#include "src/graphic/texture/texture_controller.h"
#include "src/game/mesh/layer_mesh.h"
#include "src/game/mesh/sprite_mesh.h"
#include "src/game/mesh/animated_sprite_mesh.h"

namespace game {
    class mesh_factory {
    public:
        mesh_factory(graphic::renderer& renderer);
        
        template <typename T, typename V, typename M>
        using enable_if_same = typename std::enable_if<std::is_same<T, V>::value, std::shared_ptr<M>>::type;
        
        template <typename T>
        enable_if_same<T, region::layer<uint32_t>, graphic::layer_mesh> new_mesh(const uint32_t id, const T& newObj) {
            auto result = std::make_shared<graphic::layer_mesh>(newObj, m_texCtrl);
            result->set_id(id);
            m_renderer.add_mesh(result);
            return result;
        }
    private:      
        graphic::renderer& m_renderer;
        graphic::texture_controller m_texCtrl;
    };
}
