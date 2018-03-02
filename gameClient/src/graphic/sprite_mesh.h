#ifndef gameClient_graphic_sprite_mesh_h
#define gameClient_graphic_sprite_mesh_h

#include "src/region/vec3.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/shader.h"
#include "src/graphic/base_mesh.h"
#include "src/graphic/base_texture.h"
#include "src/graphic/img_texture.h"

namespace graphic {
    class sprite_mesh : public base_mesh {
    public:
        sprite_mesh(const region::dynamic_obj* obj) : m_texture("../res/tile.png"), m_obj(*obj) {
            set_position(region::vec3<float>(50, 50, 0));
        }

        void load() {
            build_position();
            build_uv();

            add_texture(m_texture);
            m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../shader/sprite_vertex.glsl");
            m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../shader/sprite_fragment.glsl");

            m_shaders.add_uniform_attr("tex");
            m_shaders.add_uniform_attr("scale");
            m_shaders.add_uniform_attr("position");
            m_shaders.link();
        }

        void free() {
            
        }

        void update() {
            m_shaders.set_uniform_attr<int>("tex", 0);
            m_shaders.set_uniform_attr<float>("scale", 0.5, 0.5);
            m_shaders.set_uniform_attr<float>("position", m_position.x, m_position.y, m_position.z);
        }
        
        void set_position(const region::vec3<float>& newPos) {
            int resolution = 128;
            float size = 0.005;
            float scale = 0;//0.01;            
            float depthMargin = 1.0f / (2.0f * resolution);
            
            
            float depthTL = -((newPos.x + newPos.y) * depthMargin);

            m_position.x = -(newPos.x - newPos.y)     * 2 * size;
            m_position.y = -((newPos.x + newPos.y)     * size - 0);
            m_position.z = depthTL;
        }
        

    private:
        void build_position() {
            set_vertex_attr_dimension(0, 3);

            add_vertex_attr(0, 0);
            add_vertex_attr(0, 0);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 0);
            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 1);
            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 0);
            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);
        }

        void build_uv() {
            set_vertex_attr_dimension(1, 2);

            add_vertex_attr(1, 0);
            add_vertex_attr(1, 0);

            add_vertex_attr(1, 1);
            add_vertex_attr(1, 0);

            add_vertex_attr(1, 0);
            add_vertex_attr(1, 1);

            add_vertex_attr(1, 1);
            add_vertex_attr(1, 1);

            add_vertex_attr(1, 1);
            add_vertex_attr(1, 0);

            add_vertex_attr(1, 0);
            add_vertex_attr(1, 1);
        }

        region::vec3<float> m_position;
        img_texture m_texture;
        const region::dynamic_obj& m_obj;
    };
}


#endif

