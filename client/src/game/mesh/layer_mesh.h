#pragma once

#include <vector>
#include <unordered_set>
#include <stdint.h>
#include <random>

#include "src/region/layer.h"
#include "src/graphic/shader/shader.h"
#include "src/graphic/mesh/base_mesh.h"
#include "src/graphic/texture/img_texture.h"
#include "src/graphic/texture/data_texture.h"
#include "src/graphic/texture/texture_controller.h"

namespace graphic {
    class layer_mesh : public base_mesh {
    public:
        constexpr static const int resolution = 512;

        layer_mesh(const region::layer<uint32_t>& layer, texture_controller& texCtrl, const uint32_t id);
        
        void load();
        void free();
        void update(const graphic::settings& settings);
        void build();
    private:
        float waterOffset = 0;
    };
}
