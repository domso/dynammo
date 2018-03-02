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

namespace graphic {
    class region_mesh : public base_mesh {
    public:
        constexpr static const int resolution = 128;

        region_mesh(const region::layer<uint32_t>* layer) : m_dataTexture(layer->data(), layer->size, layer->size), m_testTexture("../res/tile.png"), m_layer(*layer) {
            
        }
        
        void load() {
            build();
            
            add_texture(m_testTexture);
            add_texture(m_dataTexture);
            
            m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../shader/vertex.glsl");
            m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../shader/fragment.glsl");

            m_shaders.add_uniform_attr("groundTex");
            m_shaders.add_uniform_attr("mapData");            
            m_shaders.link();
        }

        void free() {
            
        }

        void update() {
            m_shaders.set_uniform_attr<int>("groundTex", 0);
            m_shaders.set_uniform_attr<int>("mapData", 1);
        }


        glm::vec3 calc_normal(const int x, const int y, const float scale, const float size) {
            float center = m_layer.get_nearest(x, y) * scale;
            float top = m_layer.get_nearest(x, y - 1) * scale;
            float bot = m_layer.get_nearest(x, y + 1) * scale;
            float left = m_layer.get_nearest(x - 1, y) * scale;
            float right = m_layer.get_nearest(x + 1, y) * scale;

            glm::vec3 result;
            result.x = -(right - left);
            result.y = -(bot - top);
            result.z = 2;

            return glm::normalize(result);
        }

        void build() {
            float size = 0.005;
            float scale = 0;//0.01;

            float depthMargin = 1.0f / (2.0f * resolution);
            
            set_vertex_attr_dimension(0, 3);
            set_vertex_attr_dimension(1, 2);

            for (int y = 1; y < resolution -1; y++) {
                for (int x = 1; x < resolution -1; x++) {
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
                    
                    //Tile naming
                    //|TL|TR|
                    //|BL|BR|

                    /*
                    float offset = 0;

                    float heightTL = m_layer.get(x, y) * scale;
                    float heightTR = m_layer.get(x + 1, y) * scale;
                    float heightBL = m_layer.get(x, y + 1) * scale;
                    float heightBR = m_layer.get(x + 1, y + 1) * scale;

                    float depthTL = -((x + y) * depthMargin);
                    float depthTR = -((x + y + 1) * depthMargin);
                    float depthBL = -((x + y + 1) * depthMargin);
                    float depthBR = -((x + y + 2) * depthMargin);

                    add_vertex_attr(0, -(x - y)     * 2 * size);
                    add_vertex_attr(0, -((x + y)     * size - heightTL - offset));
                    add_vertex_attr(0, depthTL);
                    add_vertex_attr(1, heightTL);
                    add_vertex_attr(2, 0);
                    add_vertex_attr(2, 0.5);

                    add_vertex_attr(0, -(x - y + 1) * 2 * size);
                    add_vertex_attr(0, -((x + y + 1) * size - heightTR - offset));
                    add_vertex_attr(0, depthTR);
                    add_vertex_attr(1, heightTR);
                    add_vertex_attr(2, 0.5);
                    add_vertex_attr(2, 1);

                    add_vertex_attr(0, -(x - y - 1) * 2 * size);
                    add_vertex_attr(0, -((x + y + 1) * size - heightBL - offset));
                    add_vertex_attr(0, depthBL);
                    add_vertex_attr(1, heightBL);
                    add_vertex_attr(2, 0.5);
                    add_vertex_attr(2, 0);

                    add_vertex_attr(0, -(x - y)     * 2 * size);
                    add_vertex_attr(0, -((x + y + 2)     * size - heightBR - offset));
                    add_vertex_attr(0, depthBR);
                    add_vertex_attr(1, heightBR);
                    add_vertex_attr(2, 1);
                    add_vertex_attr(2, 0.5);

                    add_vertex_attr(0, -(x - y + 1) * 2 * size);
                    add_vertex_attr(0, -((x + y + 1) * size - heightTR - offset));
                    add_vertex_attr(0, depthTR);
                    add_vertex_attr(1, heightTR);
                    add_vertex_attr(2, 0.5);
                    add_vertex_attr(2, 1);

                    add_vertex_attr(0, -(x - y - 1) * 2 * size);
                    add_vertex_attr(0, -((x + y + 1) * size - heightBL - offset));
                    add_vertex_attr(0, depthBL);
                    add_vertex_attr(1, heightBL);
                    add_vertex_attr(2, 0.5);
                    add_vertex_attr(2, 0);


                    glm::vec3 v;

                    v = calc_normal(x, y, 1, size);
                    add_vertex_attr(3, v.x);
                    add_vertex_attr(3, v.y);
                    add_vertex_attr(3, v.z);

                    v = calc_normal(x + 1, y, 1, size);
                    add_vertex_attr(3, v.x);
                    add_vertex_attr(3, v.y);
                    add_vertex_attr(3, v.z);

                    v = calc_normal(x, y + 1, 1, size);
                    add_vertex_attr(3, v.x);
                    add_vertex_attr(3, v.y);
                    add_vertex_attr(3, v.z);



                    v = calc_normal(x + 1, y + 1, 1, size);
                    add_vertex_attr(3, v.x);
                    add_vertex_attr(3, v.y);
                    add_vertex_attr(3, v.z);

                    v = calc_normal(x + 1, y, 1, size);
                    add_vertex_attr(3, v.x);
                    add_vertex_attr(3, v.y);
                    add_vertex_attr(3, v.z);

                    v = calc_normal(x, y + 1, 1, size);
                    add_vertex_attr(3, v.x);
                    add_vertex_attr(3, v.y);
                    add_vertex_attr(3, v.z);
                    */
                }

            }

        }
        
        data_texture m_dataTexture;
        img_texture m_testTexture;
        const region::layer<uint32_t>& m_layer;
    };
}


#endif
