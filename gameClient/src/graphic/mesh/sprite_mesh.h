#ifndef gameClient_graphic_sprite_mesh_h
#define gameClient_graphic_sprite_mesh_h

#include <mutex>

#include "src/region/vec3.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/shader.h"
#include "src/graphic/mesh/base_mesh.h"
#include "src/graphic/texture/base_texture.h"
#include "src/graphic/texture/img_texture.h"
#include "src/graphic/texture/data_texture.h"
#include "src/graphic/texture/texture_controller.h"

namespace graphic {
    class sprite_mesh : public base_mesh {
    public:
        sprite_mesh(const std::vector<region::static_obj>* objs, texture_controller& texCtrl, const region::layer<uint32_t>* layer) :
            m_dataTexture(texCtrl.load_data_texture(layer->data(), layer->width, layer->height))

        {
            m_objs = *objs;
            m_texture = texCtrl.load_img_texture("../res/Sprites/firtree.png");
//             if (obj->type == 0) {
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/castle.png");
//             } else if (obj->type == 1){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/chapel.png");
//             }else if (obj->type == 2){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/clock.png");
//             }else if (obj->type == 3){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/crops.png");
//             }else if (obj->type == 4){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/dirt.png");
//             }else if (obj->type == 5){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/marblepath.png");
//             }else if (obj->type == 6){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/fish.png");
//             }else if (obj->type == 7){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/fruit.png");
//             }else if (obj->type == 8){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/house.png");
//             }else if (obj->type == 9){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/inn.png");
//             }else if (obj->type == 10){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/firtree.png");
//             }else if (obj->type == 11){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/oaktree.png");
//             }else if (obj->type == 12){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/pinetree.png");
//             }else if (obj->type == 13){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/tailor.png");
//             }else if (obj->type == 14){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/tavern.png");
//             }else if (obj->type == 15){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/thatched.png");
//             }else if (obj->type == 16){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/treehouse.png");
//             }else if (obj->type == 17){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/villa.png");
//             }else if (obj->type == 18){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0000.png");
//             }else if (obj->type == 19){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0001.png");
//             }else if (obj->type == 20){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0002.png");
//             }else if (obj->type == 21){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0003.png");
//             }else if (obj->type == 22){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0004.png");
//             }else if (obj->type == 23){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0005.png");
//             }else if (obj->type == 24){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0006.png");
//             }else if (obj->type == 25){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0007.png");
//             }else if (obj->type == 26){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0008.png");
//             }else if (obj->type == 27){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0009.png");
//             }else if (obj->type == 28){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/wal0010.png");
//             }else if (obj->type == 29){
//                 m_texture = texCtrl.load_img_texture("../res/Sprites/watchtower.png");
//             }
//
//
//             set_position(obj->position);
        }

        sprite_mesh(const region::dynamic_obj*  obj, texture_controller& texCtrl, const region::layer<uint32_t>* layer) :
            m_dataTexture(texCtrl.load_data_texture(layer->data(), layer->width, layer->height))
//         m_texture(texCtrl.load_img_texture("../res/tile.png"))
//         m_texture(texCtrl.load_img_texture("../res/Sprites/watchtower.png"))
        {
            m_texture = texCtrl.load_img_texture("../res/Sprites/watchtower.png");
            set_position(obj->position);
        }

        void load() {
            std::lock_guard<std::mutex> lg(m_mutex);
            build_vertex();
            build_uv();
            build_position();
            
            add_texture(*m_texture.get());
            add_texture(*m_dataTexture.get());

            m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../shader/sprite_vertex.glsl");
            m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../shader/sprite_fragment.glsl");

            m_shaders.add_uniform_attr("tex");
            m_shaders.add_uniform_attr("mapData");
            m_shaders.add_uniform_attr("scale");
            m_shaders.add_uniform_attr("camera");
            m_shaders.add_uniform_attr("screenResolution");
            m_shaders.add_uniform_attr("zoom");
//             m_shaders.add_uniform_attr("position");
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
            m_shaders.set_uniform_attr<float>("scale", 0.025, 0.025 * ((double) m_texture->height() / m_texture->width()));
            m_shaders.set_uniform_attr<float>("zoom", settings.zoomX, settings.zoomY);
            m_shaders.set_uniform_attr<float>("screenResolution", settings.currentWidth, settings.currentHeight);
            m_shaders.set_uniform_attr<float>("camera", settings.cameraX, settings.cameraY);

//             m_shaders.set_uniform_attr<float>("position", m_position.x, m_position.y, m_position.z);
        }

        void set_position(const region::vec3<uint16_t>& newPos) {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_position = newPos.convertTo<float>();
        }

        void set_position(const region::vec3<float>& newPos) {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_position = newPos;
        }

    private:
        void build_vertex() {
            set_vertex_attr_dimension(0, 3);

            for (size_t i = 0; i < m_objs.size(); i++) {
                add_vertex_attr(0, -0.5);
                add_vertex_attr(0, 0);
                add_vertex_attr(0, 0);

                add_vertex_attr(0, 0.5);
                add_vertex_attr(0, 0);
                add_vertex_attr(0, 0);

                add_vertex_attr(0, -0.5);
                add_vertex_attr(0, 1);
                add_vertex_attr(0, 1);

                add_vertex_attr(0, 0.5);
                add_vertex_attr(0, 1);
                add_vertex_attr(0, 1);

                add_vertex_attr(0, 0.5);
                add_vertex_attr(0, 0);
                add_vertex_attr(0, 0);

                add_vertex_attr(0, -0.5);
                add_vertex_attr(0, 1);
                add_vertex_attr(0, 1);
            }
        }

        void build_uv() {
            set_vertex_attr_dimension(1, 2);

            for (size_t i = 0; i < m_objs.size(); i++) {
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
        }

        void build_position() {
            set_vertex_attr_dimension(2, 3);

            for (size_t i = 0; i < m_objs.size(); i++) {
                add_vertex_attr(2, m_objs[i].position.x);
                add_vertex_attr(2, m_objs[i].position.y);
                add_vertex_attr(2, m_objs[i].position.z);
                
                add_vertex_attr(2, m_objs[i].position.x);
                add_vertex_attr(2, m_objs[i].position.y);
                add_vertex_attr(2, m_objs[i].position.z);
                
                add_vertex_attr(2, m_objs[i].position.x);
                add_vertex_attr(2, m_objs[i].position.y);
                add_vertex_attr(2, m_objs[i].position.z);
                
                add_vertex_attr(2, m_objs[i].position.x);
                add_vertex_attr(2, m_objs[i].position.y);
                add_vertex_attr(2, m_objs[i].position.z);
                
                add_vertex_attr(2, m_objs[i].position.x);
                add_vertex_attr(2, m_objs[i].position.y);
                add_vertex_attr(2, m_objs[i].position.z);
                
                add_vertex_attr(2, m_objs[i].position.x);
                add_vertex_attr(2, m_objs[i].position.y);
                add_vertex_attr(2, m_objs[i].position.z);
            }
        }

        std::mutex m_mutex;
        std::vector<region::static_obj> m_objs;
        region::vec3<float> m_position;
        std::shared_ptr<data_texture> m_dataTexture;
        std::shared_ptr<img_texture> m_texture;
    };
}


#endif

