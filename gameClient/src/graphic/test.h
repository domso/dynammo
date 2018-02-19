
#include <iostream>
#include <string>
#include <vector>

#include <gtkmm.h>
#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"


class Example_GLArea : public Gtk::Window {
public:
    Example_GLArea();
    ~Example_GLArea() override;

    void add_mesh(graphic::mesh* newMesh) {
        m_renderMeshes.push_back(newMesh);
    }
protected:
    Gtk::Box m_VBox ;
    Gtk::GLArea m_GLArea;

    void realize();
    void unrealize();
    bool render(const Glib::RefPtr<Gdk::GLContext>& context);

    std::vector<graphic::mesh*> m_renderMeshes;
};


Example_GLArea::Example_GLArea() {
    set_title("GL Area");
    set_default_size(400, 600);

    m_VBox.property_margin() = 12;
    m_VBox.set_spacing(6);
    add(m_VBox);

    m_GLArea.set_hexpand(true);
    m_GLArea.set_vexpand(true);
    m_GLArea.set_auto_render(true);
    m_VBox.add(m_GLArea);

    // Connect gl area signals
    m_GLArea.signal_realize().connect(sigc::mem_fun(*this, &Example_GLArea::realize));
    // Important that the unrealize signal calls our handler to clean up
    // GL resources _before_ the default unrealize handler is called (the "false")
    m_GLArea.signal_unrealize().connect(sigc::mem_fun(*this, &Example_GLArea::unrealize), false);
    m_GLArea.signal_render().connect(sigc::mem_fun(*this, &Example_GLArea::render), false);

    m_GLArea.set_has_depth_buffer(true);

    m_VBox.show();
    m_GLArea.show();
}

Example_GLArea::~Example_GLArea() {

}

void Example_GLArea::realize() {
    m_GLArea.make_current();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    for (graphic::mesh* currentMesh : m_renderMeshes) {
        currentMesh->realize();
    }

    if (m_GLArea.has_error()) {
        std::cout << "An error occured making the context current during realize:" << std::endl;
//         std::cout << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}

void Example_GLArea::unrealize() {
    m_GLArea.make_current();

    if (m_GLArea.has_error()) {
        std::cerr << "An error occured making the context current during unrealize" << std::endl;
//         std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
    
    for (graphic::mesh* currentMesh : m_renderMeshes) {
        currentMesh->unrealize();
    }
}

bool Example_GLArea::render(const Glib::RefPtr<Gdk::GLContext>& /* context */) {
    if (m_GLArea.has_error()) {
        std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
//             std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
        return false;
    }

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    for (graphic::mesh* currentMesh : m_renderMeshes) {
        currentMesh->render();
    }


    glFlush();
    
    m_GLArea.queue_draw();
    
//     THIS->DRAW();   
//     GTK::WIDGET::QUEUE_DRAW()
    
    return true;
}

// int main(int argc, char* argv[]) {
//     auto app = Gtk::Application::create(argc, argv);
//
//     Example_GLArea test;
//     graphic::mesh newMesh;
//
//
//     test.add_graphic::mesh(&newMesh);
//
//     //Shows the window and returns when it is closed.
//     return app->run(test);
// }

