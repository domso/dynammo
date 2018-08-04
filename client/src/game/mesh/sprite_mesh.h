#pragma once

#include "src/region/vec3.h"
#include "src/region/dynamic_obj.h"
#include "src/region/static_obj.h"
#include "src/graphic/shader/shader.h"
#include "src/graphic/mesh/base_mesh.h"
#include "src/graphic/texture/base_texture.h"
#include "src/graphic/texture/img_texture.h"
#include "src/graphic/texture/data_texture.h"
#include "src/graphic/texture/texture_controller.h"

namespace graphic {
    class sprite_mesh : public base_mesh {
    public:
        sprite_mesh(const std::vector<region::static_obj>& objs, texture_controller& texCtrl);
        void load();
        void free();
        void update(const graphic::settings& settings);

    private:
        void build_vertex();
        void build_uv();
        void build_position();        
        void build_type();
        
        std::vector<region::static_obj> m_objs;
    };
}
