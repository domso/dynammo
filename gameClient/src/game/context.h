#pragma once

#include <vector>
#include <unordered_map>
#include <type_traits>
#include "src/game/mesh/layer_mesh.h"
#include "src/game/mesh/sprite_mesh.h"
#include "src/game/mesh/animated_sprite_mesh.h"
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
            
            for (size_t i = 0; i < 1; i++) {
                m_layers[i] = m_meshFactory.new_mesh(i, layers[i]);                
            }
        }
        
        template <typename T>
        enable_if_same<T, std::vector<region::static_obj>> add_game_object(const T& sprites) {           
            m_sprites.push_back(m_meshFactory.new_mesh(0, sprites));
        }
        
        template <typename T>
        enable_if_same<T, std::vector<region::dynamic_obj>> add_game_object(const T& animatedSprites) {      
            for (auto& obj : animatedSprites) {
                add_game_object(obj);
            }            
        }
        
        template <typename T>
        enable_if_same<T, region::dynamic_obj> add_game_object(const T& animatedSprites) {           
            auto it = m_animatedSprites.find(animatedSprites.id);
            
            if (it != m_animatedSprites.end()) {
                m_animatedSprites[animatedSprites.id]->update_data(&animatedSprites);
            } else {
                m_animatedSprites[animatedSprites.id] = m_meshFactory.new_mesh(0, animatedSprites);
            }           
            
        }
         
    private:
        game::mesh_factory& m_meshFactory;
        std::vector<std::shared_ptr<graphic::layer_mesh>> m_layers;
        std::vector<std::shared_ptr<graphic::sprite_mesh>> m_sprites;
        std::unordered_map<uint32_t, std::shared_ptr<graphic::animated_sprite_mesh>> m_animatedSprites;
    };   
}
