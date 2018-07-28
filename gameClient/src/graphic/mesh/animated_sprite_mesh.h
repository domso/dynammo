#pragma once

#include <mutex>
#include <chrono>

#include "src/region/vec3.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/shader.h"
#include "src/graphic/mesh/base_mesh.h"
#include "src/graphic/texture/base_texture.h"
#include "src/graphic/texture/img_texture.h"
#include "src/graphic/texture/img_block_texture.h"
#include "src/graphic/texture/data_texture.h"
#include "src/graphic/texture/texture_controller.h"

namespace graphic {
    class animated_sprite_mesh : public base_mesh {
    public:        
        animated_sprite_mesh(const region::dynamic_obj* obj, texture_controller& texCtrl, const region::layer<uint32_t>* layer) :         
        m_dataTexture(texCtrl.load_data_texture(layer->data(), layer->width, layer->height)),
//         m_texture(texCtrl.load_img_texture("../res/tile.png"))        
        m_texture(texCtrl.load_img_texture("../res/Sprites/CitizenSheet.png"))
//         m_textureBlock(texCtrl.load_img_block_texture("../res/isometric_Mini-Crusader/walk/crusader_walk_", 0, 119, 15))
        {
            set_position(obj->position);
            m_currentFrame = 0;
            m_firstFrame = 0;
            m_lastFrame = 0;            
        }

        void load() {
            std::lock_guard<std::mutex> lg(m_mutex);
            build_position();
            build_uv();

            add_texture(*m_texture.get());
            add_texture(*m_dataTexture.get());
            
            m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../shader/animated_sprite_vertex.glsl");
            m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../shader/sprite_fragment.glsl");

            m_shaders.add_uniform_attr("tex");
            m_shaders.add_uniform_attr("mapData");
            m_shaders.add_uniform_attr("scale");
            m_shaders.add_uniform_attr("camera");
            m_shaders.add_uniform_attr("screenResolution");
            m_shaders.add_uniform_attr("zoom");
            m_shaders.add_uniform_attr("position");
            m_shaders.add_uniform_attr("frameDimension");
            m_shaders.add_uniform_attr("frame");
            m_shaders.add_uniform_attr("frameInvert");
            
            
            
            m_shaders.link();
        }

        void free() {
            std::lock_guard<std::mutex> lg(m_mutex);            
        }     
        
        void update_data(const region::dynamic_obj* obj) {
            set_position(obj->position); 
             switch (obj->animation) {
//                 case types::game_animations::move_up:    set_animation(60, 74, false);    break;
//                 case types::game_animations::move_left:  set_animation(90, 104, false);  break;
//                 case types::game_animations::move_down:  set_animation(0, 14, false);  break;
//                 case types::game_animations::move_right: set_animation(30, 44, false); break;
                 
                case types::game_animations::move_up:    set_animation(4, 7, true, false, 2);    break;
                case types::game_animations::move_left:  set_animation(4, 7, true, true, 2);  break;
                case types::game_animations::move_down:  set_animation(0, 3, true, true, 2);  break;
                case types::game_animations::move_right: set_animation(0, 3, true, false, 2); break;
                default: break;
            }    
        }

        void update(const graphic::settings& settings) {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_shaders.set_uniform_attr<int>("tex", 0);
            m_shaders.set_uniform_attr<int>("mapData", 1);
            m_shaders.set_uniform_attr<float>("scale", 0.01, 0.01);
            m_shaders.set_uniform_attr<float>("zoom", settings.zoomX, settings.zoomY);
            m_shaders.set_uniform_attr<float>("screenResolution", settings.currentWidth, settings.currentHeight);
            m_shaders.set_uniform_attr<float>("camera", settings.cameraX, settings.cameraY);
            
            m_shaders.set_uniform_attr<float>("position", m_position.x, m_position.y, m_position.z);
            
            m_shaders.set_uniform_attr<int>("frameDimension", 15);
            
            m_shaders.set_uniform_attr<int>("frame", (int)m_currentFrame);
            m_shaders.set_uniform_attr<int>("frameInvert", m_invert);
            
            
            m_currentFrame += m_speed * settings.frameDuration;  
            if (m_currentFrame >= m_lastFrame + 1) {
                m_currentFrame = m_lastFrame;
                if (m_loopFrame) {
                    m_currentFrame = m_firstFrame;                    
                }                
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
        
        void set_animation(const int first, const int last, const bool loop, const bool invert, const float speed) {            
            if (m_currentFrame < first || last < m_currentFrame) {
                m_currentFrame = first;
            }
            
            m_firstFrame = first;
            m_lastFrame = last;
            m_loopFrame = loop;
            m_invert = invert;
            m_speed = speed;
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
        float m_currentFrame;
        int m_firstFrame;
        int m_lastFrame;
        float m_speed;
        bool m_loopFrame;
        bool m_invert;
        
        std::shared_ptr<img_block_texture> m_textureBlock;
    };
}
