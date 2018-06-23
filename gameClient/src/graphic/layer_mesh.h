#ifndef gameClient_graphic_region_mesh_h
#define gameClient_graphic_region_mesh_h

#include <vector>
#include <unordered_set>
#include <stdint.h>
#include <random>

#include "src/region/layer.h"
#include "src/graphic/shader.h"
#include "src/graphic/base_mesh.h"
#include "src/graphic/img_texture.h"
#include "src/graphic/data_texture.h"
#include "src/graphic/texture_controller.h"

namespace graphic {
    class layer_mesh : public base_mesh {
    public:
        constexpr static const int resolution = 512;

        layer_mesh(const region::layer<uint32_t>* layer, texture_controller& texCtrl) :
            m_dataTexture(texCtrl.load_data_texture(layer->data(), layer->width, layer->height)),
            m_testTexture(texCtrl.load_img_texture("../res/newgrass.png")) {

        }

        void load() {
            build();

            add_texture(*m_testTexture.get());
            add_texture(*m_dataTexture.get());

            m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../shader/vertex.glsl");
            m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../shader/fragment.glsl");

            m_shaders.add_uniform_attr("groundTex");
            m_shaders.add_uniform_attr("mapData");
            m_shaders.add_uniform_attr("regionID");
            m_shaders.add_uniform_attr("zoom");
            m_shaders.add_uniform_attr("camera");
            m_shaders.add_uniform_attr("screenResolution");
            m_shaders.link();
        }

        void free() {

        }

        void update_data(const region::layer<uint32_t>* layer) {

        }

        void update(const graphic::settings& settings) {
            m_shaders.set_uniform_attr<int>("groundTex", 0);
            m_shaders.set_uniform_attr<int>("mapData", 1);
            m_shaders.set_uniform_attr<int>("regionID", get_id());
            m_shaders.set_uniform_attr<float>("zoom", settings.zoomX, settings.zoomY);
            m_shaders.set_uniform_attr<float>("screenResolution", settings.currentWidth, settings.currentHeight);
            m_shaders.set_uniform_attr<float>("camera", settings.cameraX, settings.cameraY);

        }

        void build() {
            set_vertex_attr_dimension(0, 3);
            set_vertex_attr_dimension(1, 2);

            for (int y = 0; y < resolution; y++) {
                for (int x = 0; x < resolution; x++) {
                    add_vertex_attr(0, x);
                    add_vertex_attr(0, y);
                    add_vertex_attr(0, 0);
                    add_vertex_attr(1, 0);
                    add_vertex_attr(1, 0.5);

                    add_vertex_attr(0, x + 1);
                    add_vertex_attr(0, y);
                    add_vertex_attr(0, 0);
                    add_vertex_attr(1, 0.5);
                    add_vertex_attr(1, 1);

                    add_vertex_attr(0, x);
                    add_vertex_attr(0, y + 1);
                    add_vertex_attr(0, 0);
                    add_vertex_attr(1, 0.5);
                    add_vertex_attr(1, 0);

                    add_vertex_attr(0, x + 1);
                    add_vertex_attr(0, y + 1);
                    add_vertex_attr(0, 0);
                    add_vertex_attr(1, 1);
                    add_vertex_attr(1, 0.5);

                    add_vertex_attr(0, x + 1);
                    add_vertex_attr(0, y);
                    add_vertex_attr(0, 0);
                    add_vertex_attr(1, 0.5);
                    add_vertex_attr(1, 1);

                    add_vertex_attr(0, x);
                    add_vertex_attr(0, y + 1);
                    add_vertex_attr(0, 0);
                    add_vertex_attr(1, 0.5);
                    add_vertex_attr(1, 0);
                }

            }

        }

        std::shared_ptr<data_texture> m_dataTexture;
        std::shared_ptr<img_texture> m_testTexture;
    };
}


#endif
