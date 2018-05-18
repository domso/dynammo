#pragma once

#include <stdint.h>
#include <unordered_map>
#include <type_traits>

#include "src/region/layer.h"
#include "src/region/static_obj.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/renderer.h"
#include "src/graphic/texture_controller.h"
#include "src/graphic/layer_mesh.h"
#include "src/graphic/sprite_mesh.h"
#include "src/graphic/animated_sprite_mesh.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"

namespace graphic {
    class region_mesh_controller {
    public:
        region_mesh_controller(const uint32_t regionID, graphic::renderer& renderer, graphic::texture_controller& texCtrl);

        template <typename T, typename argT>
        void add_mesh(const T* newObj, const uint32_t id, const argT* arg) {
            auto newMesh = container<T>().emplace(id, new_mesh(newObj, arg)).first->second;
            newMesh->set_id(id);
            m_renderer.add_mesh(newMesh);
        }

        template <typename T>
        void remove_mesh(const uint32_t id) {
            auto it = container<T>().find(id);
            m_renderer.remove_mesh(it->second);
            container<T>().erase(it);
        }

        template <typename T>
        void update_mesh(const T* newObj, const uint32_t id) {
            auto& mesh = container<T>().find(id)->second;
            mesh->update_data(newObj);
        }
    private:
        template <typename T>
        typename std::enable_if<std::is_same<T, region::layer<uint32_t>>::value, std::unordered_map<uint32_t, std::shared_ptr<layer_mesh>>&>::type container() {
            return m_layerMeshes;
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, region::static_obj>::value, std::unordered_map<uint32_t, std::shared_ptr<sprite_mesh>>&>::type container() {
            return m_spriteMeshes;
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, region::dynamic_obj>::value, std::unordered_map<uint32_t, std::shared_ptr<animated_sprite_mesh>>&>::type container() {
            return m_animatedSpriteMeshes;
        }

        template <typename T, typename argT = T>
        typename std::enable_if<std::is_same<T, region::layer<uint32_t>>::value, std::shared_ptr<layer_mesh>>::type new_mesh(const T* newObj, const argT* arg) {
            return std::make_shared<layer_mesh>(newObj, m_texCtrl);
        }

        template <typename T, typename argT = T>
        typename std::enable_if<std::is_same<T, region::static_obj>::value, std::shared_ptr<sprite_mesh>>::type new_mesh(const T* newObj, const argT* arg) {
            return std::make_shared<sprite_mesh>(newObj, m_texCtrl, arg);
        }

        template <typename T, typename argT = T>
        typename std::enable_if<std::is_same<T, region::dynamic_obj>::value, std::shared_ptr<animated_sprite_mesh>>::type new_mesh(const T* newObj, const argT* arg) {
            return std::make_shared<animated_sprite_mesh>(newObj, m_texCtrl, arg);
        }

        std::unordered_map<uint32_t, std::shared_ptr<layer_mesh>> m_layerMeshes;
        std::unordered_map<uint32_t, std::shared_ptr<sprite_mesh>> m_spriteMeshes;
        std::unordered_map<uint32_t, std::shared_ptr<animated_sprite_mesh>> m_animatedSpriteMeshes;

        uint32_t m_regionID;

        graphic::renderer& m_renderer;
        graphic::texture_controller& m_texCtrl;
    };
}
