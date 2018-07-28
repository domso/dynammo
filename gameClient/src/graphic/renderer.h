#ifndef gameClient_graphic_renderer_h
#define gameClient_graphic_renderer_h

#include <mutex>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <unordered_set>
#include <queue>
#include <thread>
#include <chrono>

#include "src/util/state_machine.h"
#include "src/graphic/mesh/base_mesh.h"
#include "src/graphic/settings.h"

namespace graphic {
    class renderer {
    public:
        renderer();
        renderer(const renderer& copy) = delete;
        renderer(renderer&& move) = delete;
        
        void link_glarea(Gtk::GLArea& glarea);
        void add_mesh(const std::shared_ptr<graphic::base_mesh>& newMesh);
        void remove_mesh(const std::shared_ptr<graphic::base_mesh>& oldMesh);
        
        enum class states {
            unrealized,
            realized,
            closing,
            error
        };
        
        void close();
        
        util::state_machine<states> currentState = states::unrealized;
        
        graphic::settings unpublishedSettings;
        void publish_settings();
    private:
        graphic::settings m_currentSettings;
        
        void realize();
        void unrealize();
        bool render(const Glib::RefPtr<Gdk::GLContext>& context);
        
        void render_meshes();
        void update_meshes();
        void add_new_mesh();
        void remove_old_mesh();      
        void remove_all_meshes();
        
        std::chrono::time_point<std::chrono::high_resolution_clock> lastRender;
        
        std::mutex m_mutex;
        Gtk::GLArea* m_glarea;
        std::unordered_set<std::shared_ptr<graphic::base_mesh>> m_renderMeshes;
        std::queue<std::shared_ptr<graphic::base_mesh>> m_addQueue;
        std::queue<std::shared_ptr<graphic::base_mesh>> m_removeQueue;
    };
}

#endif
