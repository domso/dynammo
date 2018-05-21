#ifndef gameClient_graphic_sprite_mesh_h
#define gameClient_graphic_sprite_mesh_h

#include <mutex>

#include "src/region/vec3.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/shader.h"
#include "src/graphic/base_mesh.h"
#include "src/graphic/base_texture.h"
#include "src/graphic/img_texture.h"
#include "src/graphic/data_texture.h"
#include "src/graphic/texture_controller.h"

namespace graphic {
    class sprite_mesh : public base_mesh {
    public:
        sprite_mesh(const region::static_obj* obj, texture_controller& texCtrl, const region::layer<uint32_t>* layer) :         
        m_dataTexture(texCtrl.load_data_texture(layer->data(), layer->size, layer->size)),
        m_texture(texCtrl.load_img_texture("../res/_tree_01/_tree_01_00000.png"))
        {
            set_position(obj->position);
        }
        
        sprite_mesh(const region::dynamic_obj*  obj, texture_controller& texCtrl, const region::layer<uint32_t>* layer) :         
        m_dataTexture(texCtrl.load_data_texture(layer->data(), layer->size, layer->size)),
//         m_texture(texCtrl.load_img_texture("../res/tile.png"))        
        m_texture(texCtrl.load_img_texture("../res/_tree_01/_tree_01_00000.png"))
        {
            set_position(obj->position);
        }

        void load() {
            std::lock_guard<std::mutex> lg(m_mutex);
            build_position();
            build_uv();

            add_texture(*m_texture.get());
            add_texture(*m_dataTexture.get());
            
            m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../shader/sprite_vertex.glsl");
            m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../shader/sprite_fragment.glsl");

            m_shaders.add_uniform_attr("tex");
            m_shaders.add_uniform_attr("mapData");
            m_shaders.add_uniform_attr("scale");
            m_shaders.add_uniform_attr("zoom");
            m_shaders.add_uniform_attr("camera");
            m_shaders.add_uniform_attr("screenResolution");
            m_shaders.add_uniform_attr("position");
            m_shaders.link();
        }

        void free() {
            std::lock_guard<std::mutex> lg(m_mutex);            
        }       
        
        void update_data(const region::dynamic_obj* obj) {
            set_position(obj->position);
        }    
        
        void update_data(const region::static_obj* obj) {
            set_position(obj->position);
        }
        
        void update(const graphic::settings& settings) {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_shaders.set_uniform_attr<int>("tex", 0);
            m_shaders.set_uniform_attr<int>("mapData", 1);
            m_shaders.set_uniform_attr<float>("scale", 0.025, 0.025);
            m_shaders.set_uniform_attr<float>("zoom", settings.zoomX, settings.zoomY);
            m_shaders.set_uniform_attr<float>("screenResolution", settings.currentWidth, settings.currentHeight);
            m_shaders.set_uniform_attr<float>("camera", settings.cameraX, settings.cameraY);
            
            m_shaders.set_uniform_attr<float>("position", m_position.x, m_position.y, m_position.z);
        }
        
        void set_position(const region::vec3<uint8_t>& newPos) {
            std::lock_guard<std::mutex> lg(m_mutex);            
            m_position = newPos.convertTo<float>();
        }      
        
        void set_position(const region::vec3<float>& newPos) {
            std::lock_guard<std::mutex> lg(m_mutex);            
            m_position = newPos;
        }  

    private:
        void build_position() {
            set_vertex_attr_dimension(0, 3);

            add_vertex_attr(0, -0.5);
            add_vertex_attr(0, 0);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 0.5);
            add_vertex_attr(0, 0);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, -0.5);
            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 0.5);
            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, 0.5);
            add_vertex_attr(0, 0);
            add_vertex_attr(0, 0);

            add_vertex_attr(0, -0.5);
            add_vertex_attr(0, 1);
            add_vertex_attr(0, 0);
        }

        void build_uv() {
            set_vertex_attr_dimension(1, 2);
            
            
            add_vertex_attr(1, 0);
            add_vertex_attr(1, 1);
            
            add_vertex_attr(1, 1);
            add_vertex_attr(1, 1);

            add_vertex_attr(1, 0);
            add_vertex_attr(1, 0);

                    
            add_vertex_attr(1, 1);
            add_vertex_attr(1, 0);
            
            add_vertex_attr(1, 1);
            add_vertex_attr(1, 1);

            add_vertex_attr(1, 0);
            add_vertex_attr(1, 0);
            
        }

        std::mutex m_mutex;
        region::vec3<float> m_position;
        std::shared_ptr<data_texture> m_dataTexture;
        std::shared_ptr<img_texture> m_texture;
    };
}


#endif

