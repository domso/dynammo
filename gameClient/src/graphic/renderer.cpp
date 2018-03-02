#include "src/graphic/renderer.h"

#include "src/graphic/window.h"

graphic::renderer::renderer() : m_glarea(nullptr) {

}

void graphic::renderer::link_glarea(Gtk::GLArea& glarea) {
    glarea.set_has_depth_buffer(true);
    m_glarea = &glarea;

    std::atomic_thread_fence(std::memory_order_release);

    glarea.signal_realize().connect(sigc::mem_fun(*this, &renderer::realize), false);
    // Important that the unrealize signal calls our handler to clean up
    // GL resources _before_ the default unrealize handler is called (the "false")
    glarea.signal_unrealize().connect(sigc::mem_fun(*this, &renderer::unrealize), false);
    glarea.signal_render().connect(sigc::mem_fun(*this, &renderer::render), false);
}

void graphic::renderer::add_mesh(graphic::base_mesh* newMesh) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_addQueue.push(newMesh);
}

void graphic::renderer::remove_mesh(graphic::base_mesh* oldMesh) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_removeQueue.push(oldMesh);
}

void graphic::renderer::realize() {
    m_glarea->make_current();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);



    if (m_glarea->has_error()) {
        std::cout << "An error occured making the context current during realize:" << std::endl;
        currentState.set(states::error);
    } else {
        currentState.set(states::realized);
    }
}

void graphic::renderer::unrealize() {
    m_glarea->make_current();

    for (graphic::base_mesh* currentMesh : m_renderMeshes) {
        currentMesh->unrealize();
    }
    
    if (m_glarea->has_error()) {
        std::cerr << "An error occured making the context current during unrealize" << std::endl;
        currentState.set(states::error);
    } else {
        currentState.set(states::unrealized);
    }
}

bool graphic::renderer::render(const Glib::RefPtr<Gdk::GLContext>& context) {
    if (m_glarea->has_error()) {
        std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
        currentState.set(states::error);
        //             std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
        return false;
    }

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_meshes();
    render_meshes();

    glFlush();

    m_glarea->queue_draw();
    return false;
}

void graphic::renderer::render_meshes() {
    for (graphic::base_mesh* currentMesh : m_renderMeshes) {
        currentMesh->render();
    }
}

void graphic::renderer::update_meshes() {
    std::lock_guard<std::mutex> lg(m_mutex);
    add_new_mesh();
    remove_old_mesh();
}


void graphic::renderer::add_new_mesh() {
    if (!m_addQueue.empty()) {
        auto newMesh = m_addQueue.front();
        newMesh->realize();
        m_renderMeshes.insert(newMesh);
        m_addQueue.pop();
    }
}

void graphic::renderer::remove_old_mesh() {
    if (!m_removeQueue.empty()) {
        auto oldMesh = m_removeQueue.front();
        oldMesh->unrealize();
        m_renderMeshes.erase(oldMesh);
        m_removeQueue.pop();
    }
}
