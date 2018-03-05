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
        void add_obj(const T* newObj, const uint32_t index) {
            std::lock_guard<std::mutex> lg(m_mutex);
            std::unique_ptr<base_mesh> newMesh = mesh_factory<T>(newObj);
            
            m_renderer.add_mesh(newMesh.get());
            m_meshes[index] = std::move(newMesh);            
        }

        template <typename T>
        void remove_obj(const T* oldObj, const uint32_t index) {
            std::lock_guard<std::mutex> lg(m_mutex);         
            m_renderer.remove_mesh(m_meshes[index].get());
            m_meshes.erase(index);
        }

    private:        
        template <typename T>
        typename std::enable_if<std::is_same<T, region::layer<uint32_t>>::value, std::unique_ptr<base_mesh>>::type mesh_factory(const T* newObj) {
            return std::unique_ptr<base_mesh>(new region_mesh(newObj, m_texCtrl));
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<T, region::dynamic_obj>::value, std::unique_ptr<base_mesh>>::type mesh_factory(const T* newObj) {
//             return std::unique_ptr<base_mesh>(new sprite_mesh(newObj, m_texCtrl));
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<T, std::pair<region::dynamic_obj*, region::layer<uint32_t>*>>::value, std::unique_ptr<base_mesh>>::type mesh_factory(const T* newObj) {
            return std::unique_ptr<base_mesh>(new sprite_mesh(newObj, m_texCtrl));
        }

        void thread_main();

        int m_argc;
        char** m_argv;
        util::event_controller<types::game_events>& m_eventCtrl;
        
        std::thread m_thread;

        std::mutex m_mutex;
        std::unordered_map<uint32_t, std::unique_ptr<base_mesh>> m_meshes;
        graphic::renderer m_renderer;
        graphic::texture_controller m_texCtrl;
        user_interface::window* m_window;
        Glib::RefPtr<Gtk::Application> m_app;
    };
}

#endif
