#pragma once

#include <mutex>

#include "src/region/vec3.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/shader.h"
#include "src/graphic/base_mesh.h"
#include "src/graphic/base_texture.h"
#include "src/graphic/img_texture.h"
#include "src/graphic/img_block_texture.h"
#include "src/graphic/data_texture.h"
#include "src/graphic/texture_controller.h"

namespace graphic {
    class animated_sprite_mesh : public base_mesh {
    public:        
        animated_sprite_mesh(const std::pair<region::dynamic_obj*, region::layer<uint32_t>*>* obj, texture_controller& texCtrl) :         
        m_dataTexture(texCtrl.load_data_texture(obj->second->data(), obj->second->size, obj->second->size)),
//         m_texture(texCtrl.load_img_texture("../res/tile.png"))        
//         m_texture(texCtrl.load_img_texture("../res/_tree_01/_tree_01_00000.png"))
        m_textureBlock(texCtrl.load_img_block_texture("../res/isometric_Mini-Crusader/walk/crusader_walk_", 0, 119, 15))
        {
            set_position(obj->first->position);
            m_currentFrame = 0;
            m_firstFrame = 0;
            m_lastFrame = 0;            
        }

        void load() {
            std::lock_guard<std::mutex> lg(m_mutex);
            build_position();
            build_uv();

            add_texture(*m_textureBlock.get());
            add_texture(*m_dataTexture.get());
            
            m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../shader/animated_sprite_vertex.glsl");
            m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../shader/sprite_fragment.glsl");

            m_shaders.add_uniform_attr("tex");
            m_shaders.add_uniform_attr("mapData");
            m_shaders.add_uniform_attr("scale");
            m_shaders.add_uniform_attr("position");
            m_shaders.add_uniform_attr("frameDimension");
            m_shaders.add_uniform_attr("frame");
            
            
            
            m_shaders.link();
        }

        void free() {
            std::lock_guard<std::mutex> lg(m_mutex);            
        }

        void update() {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_shaders.set_uniform_attr<int>("tex", 0);
            m_shaders.set_uniform_attr<int>("mapData", 1);
            m_shaders.set_uniform_attr<float>("scale", 0.25, 0.25);
            
            m_shaders.set_uniform_attr<float>("position", m_position.x, m_position.y, m_position.z);
            
            m_shaders.set_uniform_attr<int>("frameDimension", 15);
            
            m_shaders.set_uniform_attr<int>("frame", m_currentFrame);
            
            if (m_currentFrame == m_lastFrame) {
                if (m_loopFrame) {
                    m_currentFrame = m_firstFrame;                    
                }                
            } else {
                m_currentFrame++;
            }     
        }
        
        void set_position(const region::vec3<uint8_t>& newPos) {
            std::lock_guard<std::mutex> lg(m_mutex);            
            m_position = newPos.convertTo<float>();
        }      
        
        void set_position(const region::vec3<float>& newPos) {
            std::lock_guard<std::mutex> lg(m_mutex);            
            m_position = newPos;
        }  
        
        void set_animation(const int first, const int last, const bool loop) {
            m_currentFrame = first;
            m_firstFrame = first;
            m_lastFrame = last;
            m_loopFrame = loop;
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
        int m_currentFrame;
        int m_firstFrame;
        int m_lastFrame;
        bool m_loopFrame;
        
        std::shared_ptr<img_block_texture> m_textureBlock;
    };
}
