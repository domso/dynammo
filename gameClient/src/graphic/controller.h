#ifndef gameClient_graphic_controller_h
#define gameClient_graphic_controller_h

#include <thread>
#include <gtkmm.h>
#include <stdint.h>
#include <mutex>
#include <unordered_map>
#include <type_traits>
#include <memory>

#include "src/region/layer.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/renderer.h"
#include "src/graphic/texture_controller.h"
#include "src/graphic/region_mesh.h"
#include "src/graphic/sprite_mesh.h"
#include "src/user_interface/window.h"
#include "src/util/event_controller.h"
#include "src/types/game_events.h"

namespace graphic {
    class controller {
    public:
        controller(int argc, char* argv[], util::event_controller<types::game_events>& eventCtrl);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();

        void open();
        void wait_for_close();

        template <typename T>
        void add_obj(const T* newObj, const uint32_t id) {
            std::lock_guard<std::mutex> lg(m_mutex);
            add_mesh<T>(newObj, id);                              
        }

        template <typename T>
        void remove_obj(const uint32_t id) {
            std::lock_guard<std::mutex> lg(m_mutex);         
            remove_mesh<T>(id);
        }

        template <typename T>
        void update_obj(const T* newObj, const uint32_t id) {
            std::lock_guard<std::mutex> lg(m_mutex);
            update_mesh<T>(newObj, id);
        }
    private:        
        template <typename T>
        typename std::enable_if<std::is_same<T, region::layer<uint32_t>>::value, void>::type add_mesh(const T* newObj, const uint32_t id) {            
             base_mesh* newMesh = m_regionMeshes.emplace(id, new region_mesh(newObj, m_texCtrl)).first->second.get();
             m_renderer.add_mesh(newMesh);
        }
                
        template <typename T>
        typename std::enable_if<std::is_same<T, std::pair<region::dynamic_obj*, region::layer<uint32_t>*>>::value, void>::type add_mesh(const T* newObj, const uint32_t id) {
              base_mesh* newMesh = m_spriteMeshes.emplace(id, new sprite_mesh(newObj, m_texCtrl)).first->second.get();
             m_renderer.add_mesh(newMesh);
        }
               
        template <typename T>
        typename std::enable_if<std::is_same<T, region::layer<uint32_t>>::value, void>::type remove_mesh(const uint32_t id) {           
             auto it = m_regionMeshes.find(id);
             m_renderer.remove_mesh(it->second.get());
             m_regionMeshes.erase(it);
        }
                
        template <typename T>
        typename std::enable_if<std::is_same<T, std::pair<region::dynamic_obj*, region::layer<uint32_t>*>>::value, void>::type remove_mesh(const uint32_t id) {
             auto it = m_spriteMeshes.find(id);
             m_renderer.remove_mesh(it->second.get());
             m_spriteMeshes.erase(it);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<T, region::layer<uint32_t>>::value, void>::type update_mesh(const T* newObj, const uint32_t id) {            
             auto& mesh = m_regionMeshes.find(id)->second;
        }
                
        template <typename T>
        typename std::enable_if<std::is_same<T, region::dynamic_obj>::value, void>::type update_mesh(const T* newObj, const uint32_t id) {
             auto& mesh = m_spriteMeshes.find(id)->second;
             mesh->set_position(newObj->position);
        }
        
        void clear();
        
        void thread_main();

        int m_argc;
        char** m_argv;
        util::event_controller<types::game_events>& m_eventCtrl;        
        std::thread m_thread;
        std::mutex m_mutex;
        std::unordered_map<uint32_t, std::unique_ptr<region_mesh>> m_regionMeshes;        
        std::unordered_map<uint32_t, std::unique_ptr<sprite_mesh>> m_spriteMeshes;        
        graphic::renderer m_renderer;
        graphic::texture_controller m_texCtrl;
        user_interface::window* m_window;
        Glib::RefPtr<Gtk::Application> m_app;
    };
}

#endif
