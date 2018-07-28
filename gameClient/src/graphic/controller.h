#ifndef gameClient_graphic_controller_h
#define gameClient_graphic_controller_h

#include <thread>
#include <gtkmm.h>
#include <stdint.h>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include <memory>
#include <atomic>

#include "src/util/config_file.h"
#include "src/util/event_controller.h"
#include "src/user_interface/window.h"
#include "src/region/layer.h"
#include "src/region/static_obj.h"
#include "src/region/dynamic_obj.h"
#include "src/graphic/renderer.h"
#include "src/graphic/texture/texture_controller.h"
#include "src/graphic/mesh/region_mesh_controller.h"

namespace graphic {
    class controller {
    public:
        controller(int argc, char* argv[], util::event_controller<types::game_events>& eventCtrl, util::config_file& config);
        controller(const controller& copy) = delete;
        controller(controller&& move) = delete;
        ~controller();

        void open();
        void wait_for_close();

        template <typename T>
        void add_obj(const uint32_t regionID, const uint32_t objID, const T& newObj) {
            std::lock_guard<std::mutex> lg(m_mutex);
            get_region_meshes(regionID).add_mesh<T>(objID, newObj); 
        }
        
        std::string get_option(const std::string& key);
    private:        
        region_mesh_controller& get_region_meshes(const uint32_t regionID);                
        void clear();        
        void thread_main();

        int m_argc;
        char** m_argv;
        util::event_controller<types::game_events>& m_eventCtrl;        
        std::thread m_thread;
        std::mutex m_mutex;
        std::unordered_map<uint32_t, region_mesh_controller> m_regionMeshes;
        
        std::unique_ptr<user_interface::window> m_window;
        util::config_file& m_config;
        
        graphic::renderer m_renderer;
        graphic::texture_controller m_texCtrl;
        Glib::RefPtr<Gtk::Application> m_app;
    };
}

#endif
