#ifndef gameClient_graphic_renderer_h
#define gameClient_graphic_renderer_h

#include <mutex>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <unordered_set>
#include <queue>
#include <thread>
#include "src/util/state_machine.h"
#include "src/graphic/base_mesh.h"

namespace graphic {
    class renderer {
    public:
        renderer();
        renderer(const renderer& copy) = delete;
        renderer(renderer&& move) = delete;
        
        void link_glarea(Gtk::GLArea& glarea);
        void add_mesh(graphic::base_mesh* newMesh);
        void remove_mesh(graphic::base_mesh* oldMesh);
        
        enum states {
            unrealized,
            realized,
            closing,
            closed,
            error            
        };
        
        void close();
        
        util::state_machine<states> currentState = states::unrealized;
    private:  
        void realize();
        void unrealize();
        bool render(const Glib::RefPtr<Gdk::GLContext>& context);
        
        void render_meshes();
        void update_meshes();
        void add_new_mesh();
        void remove_old_mesh();      
        void remove_all_meshes();
        
        std::thread m_thread;
        std::mutex m_mutex;
        Gtk::GLArea* m_glarea;
        std::unordered_set<graphic::base_mesh*> m_renderMeshes;
        std::queue<graphic::base_mesh*> m_addQueue;
        std::queue<graphic::base_mesh*> m_removeQueue;
    };
}

#endif
