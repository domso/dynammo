#ifndef gameClient_data_map_h
#define gameClient_data_map_h

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <stdint.h>
#include <random>

#include "src/region/context.h"

#include "src/graphic/test.h"
#include "src/graphic/shader.h"

namespace data {
    class map {
    public:
        constexpr static const int resolution = 128;
        map(region::context& context) : m_dataContext(context), m_tex("res/tile.png") {

        }


        glm::vec3 calc_normal(const int x, const int y, const float scale, const float size) {
            float center = m_dataContext[0].get_nearest(x, y) * scale;
            float top = m_dataContext[0].get_nearest(x, y - 1) * scale;
            float bot = m_dataContext[0].get_nearest(x, y + 1) * scale;
            float left = m_dataContext[0].get_nearest(x - 1, y) * scale;
            float right = m_dataContext[0].get_nearest(x + 1, y) * scale;

            glm::vec3 result;
            result.x = -(right - left);
            result.y = -(bot - top);
            result.z = 2;

            return glm::normalize(result);
        }

        void build() {

            float size = 0.005;
            float scale = 0.01;

            float depthMargin = 1.0f / (2.0f * resolution);
            m_mesh.set_vertex_attr_dimension(0, 3);
            m_mesh.set_vertex_attr_dimension(1, 1);
            m_mesh.set_vertex_attr_dimension(2, 2);
            m_mesh.set_vertex_attr_dimension(3, 3);


            m_mesh.add_texture(&m_tex);
            m_mesh.set_shader_program(&m_shaders);




            for (int y = 0; y < resolution - 1; y++) {
                for (int x = 0; x < resolution - 1; x++) {

                    //Tile naming
                    //|TL|TR|
                    //|BL|BR|

                    float offset = 0;

                    float heightTL = m_dataContext[0].get(x, y) * scale;
                    float heightTR = m_dataContext[0].get(x + 1, y) * scale;
                    float heightBL = m_dataContext[0].get(x, y + 1) * scale;
                    float heightBR = m_dataContext[0].get(x + 1, y + 1) * scale;

                    float depthTL = -((x + y) * depthMargin);
                    float depthTR = -((x + y + 1) * depthMargin);
                    float depthBL = -((x + y + 1) * depthMargin);
                    float depthBR = -((x + y + 2) * depthMargin);

                    m_mesh.set_vertex_attr(0, -(x - y)     * 2 * size);
                    m_mesh.set_vertex_attr(0, -((x + y)     * size - heightTL - offset));
                    m_mesh.set_vertex_attr(0, depthTL);
                    m_mesh.set_vertex_attr(1, heightTL);
                    m_mesh.set_vertex_attr(2, 0);
                    m_mesh.set_vertex_attr(2, 0.5);

                    m_mesh.set_vertex_attr(0, -(x - y + 1) * 2 * size);
                    m_mesh.set_vertex_attr(0, -((x + y + 1) * size - heightTR - offset));
                    m_mesh.set_vertex_attr(0, depthTR);
                    m_mesh.set_vertex_attr(1, heightTR);
                    m_mesh.set_vertex_attr(2, 0.5);
                    m_mesh.set_vertex_attr(2, 1);

                    m_mesh.set_vertex_attr(0, -(x - y - 1) * 2 * size);
                    m_mesh.set_vertex_attr(0, -((x + y + 1) * size - heightBL - offset));
                    m_mesh.set_vertex_attr(0, depthBL);
                    m_mesh.set_vertex_attr(1, heightBL);
                    m_mesh.set_vertex_attr(2, 0.5);
                    m_mesh.set_vertex_attr(2, 0);

                    m_mesh.set_vertex_attr(0, -(x - y)     * 2 * size);
                    m_mesh.set_vertex_attr(0, -((x + y + 2)     * size - heightBR - offset));
                    m_mesh.set_vertex_attr(0, depthBR);
                    m_mesh.set_vertex_attr(1, heightBR);
                    m_mesh.set_vertex_attr(2, 1);
                    m_mesh.set_vertex_attr(2, 0.5);

                    m_mesh.set_vertex_attr(0, -(x - y + 1) * 2 * size);
                    m_mesh.set_vertex_attr(0, -((x + y + 1) * size - heightTR - offset));
                    m_mesh.set_vertex_attr(0, depthTR);
                    m_mesh.set_vertex_attr(1, heightTR);
                    m_mesh.set_vertex_attr(2, 0.5);
                    m_mesh.set_vertex_attr(2, 1);

                    m_mesh.set_vertex_attr(0, -(x - y - 1) * 2 * size);
                    m_mesh.set_vertex_attr(0, -((x + y + 1) * size - heightBL - offset));
                    m_mesh.set_vertex_attr(0, depthBL);
                    m_mesh.set_vertex_attr(1, heightBL);
                    m_mesh.set_vertex_attr(2, 0.5);
                    m_mesh.set_vertex_attr(2, 0);


                    glm::vec3 v;

                    v = calc_normal(x, y, 1, size);
                    m_mesh.set_vertex_attr(3, v.x);
                    m_mesh.set_vertex_attr(3, v.y);
                    m_mesh.set_vertex_attr(3, v.z);

                    v = calc_normal(x + 1, y, 1, size);
                    m_mesh.set_vertex_attr(3, v.x);
                    m_mesh.set_vertex_attr(3, v.y);
                    m_mesh.set_vertex_attr(3, v.z);

                    v = calc_normal(x, y + 1, 1, size);
                    m_mesh.set_vertex_attr(3, v.x);
                    m_mesh.set_vertex_attr(3, v.y);
                    m_mesh.set_vertex_attr(3, v.z);



                    v = calc_normal(x + 1, y + 1, 1, size);
                    m_mesh.set_vertex_attr(3, v.x);
                    m_mesh.set_vertex_attr(3, v.y);
                    m_mesh.set_vertex_attr(3, v.z);

                    v = calc_normal(x + 1, y, 1, size);
                    m_mesh.set_vertex_attr(3, v.x);
                    m_mesh.set_vertex_attr(3, v.y);
                    m_mesh.set_vertex_attr(3, v.z);

                    v = calc_normal(x, y + 1, 1, size);
                    m_mesh.set_vertex_attr(3, v.x);
                    m_mesh.set_vertex_attr(3, v.y);
                    m_mesh.set_vertex_attr(3, v.z);
                }

            }

        }
//     private: //TODO
        graphic::mesh m_mesh;
        graphic::texture m_tex;
        graphic::shader_program m_shaders;
        region::context& m_dataContext;
    };
}


#endif
