#include "src/graphic/renderer.h"

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

void graphic::renderer::add_mesh(const std::shared_ptr<graphic::base_mesh>& newMesh) {
    std::lock_guard<std::mutex> lg(m_mutex);
    
    m_addQueue.push(newMesh);    
    
}

void graphic::renderer::remove_mesh(const std::shared_ptr<graphic::base_mesh>& oldMesh) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_addQueue.push(oldMesh);
}

void graphic::renderer::close() {   
    if (currentState.set_from(states::realized, states::closing)) {
        unrealize();           
    }    
}

void graphic::renderer::publish_settings() {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_currentSettings = unpublishedSettings;
}


void graphic::renderer::realize() {
    m_glarea->make_current();

//     glEnable(GL_DEPTH_TEST);
//     glDepthFunc(GL_LESS);
//     glEnable(GL_BLEND);
    
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    if (m_glarea->has_error()) {
        std::cout << "An error occured making the context current during realize:" << std::endl;
        currentState.set(states::error);
    } else {
        currentState.set(states::realized);
    }
    
    lastRender = std::chrono::high_resolution_clock::now();
}

void graphic::renderer::unrealize() {
    m_glarea->make_current();
    remove_all_meshes();
    // for some reasons, this methode has no GL-context!

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

    {
        std::lock_guard<std::mutex> lg(m_mutex);
        m_currentSettings.currentWidth = m_glarea->get_allocated_width();
        m_currentSettings.currentHeight = m_glarea->get_allocated_height();
        auto currentTP = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> fsec = currentTP - lastRender;
        m_currentSettings.frameDuration = fsec.count();
        lastRender = currentTP;
        
        update_meshes();
        render_meshes();
    }
    
    glFlush();

    m_glarea->queue_draw();
    return false;
}

void graphic::renderer::render_meshes() {
    for (auto& currentMesh : m_renderMeshes) {
        currentMesh->render(m_currentSettings);
        
        if (currentMesh.use_count() == 1) {
            m_removeQueue.push(currentMesh);
        }
    }
}

void graphic::renderer::update_meshes() {
    add_new_mesh();
    remove_old_mesh();
}

void graphic::renderer::add_new_mesh() {
    while (!m_addQueue.empty()) {
        auto newMesh = m_addQueue.front();
        
        
        newMesh->realize();
        m_renderMeshes.insert(newMesh);
        
        m_addQueue.pop();
        
    }
}

void graphic::renderer::remove_old_mesh() {
    while (!m_removeQueue.empty()) {
        auto oldMesh = m_removeQueue.front();
        oldMesh->unrealize();
        m_renderMeshes.erase(oldMesh);
        m_removeQueue.pop();
    }
}

void graphic::renderer::remove_all_meshes() {
    for (auto& currentMesh : m_renderMeshes) {
        currentMesh->unrealize();
    }
    m_renderMeshes.clear();
    
    currentState.set(states::unrealized);
}
