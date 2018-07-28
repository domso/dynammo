#pragma once

#include <vector>
#include <type_traits>
#include "src/game/mesh/layer_mesh.h"
#include "src/game/mesh_factory.h"

namespace game {
    class context {
    public:
        context(game::mesh_factory& meshFactory);
        
        template <typename T, typename V>
        using enable_if_same = typename std::enable_if<std::is_same<T, V>::value, void>::type;
        
        template <typename T>
        enable_if_same<T, std::vector<region::layer<uint32_t>>> add_game_object(const T& layers) {
            m_layers.resize(layers.size());
            
            for (size_t i = 0; i < layers.size(); i++) {
                m_layers[i] = m_meshFactory.new_mesh(i, layers[i]);                
            }
        }
         
    private:
        game::mesh_factory& m_meshFactory;
        std::vector<std::shared_ptr<graphic::layer_mesh>> m_layers;
    };   
}
