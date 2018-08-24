#include "src/game/mesh/sprite_mesh.h"

graphic::sprite_mesh::sprite_mesh(const std::vector<region::static_obj>& objs, texture_controller& texCtrl) {
    m_objs = objs;
    add_texture(std::make_shared<img_texture>("../res/tiles/block/out_full.png"));
    auto layerData = texCtrl.get_texture("data::layer");
    add_texture(*layerData);
}


void graphic::sprite_mesh::load() {
    build_vertex();
    build_uv();
    build_position();
    build_type();

    m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../src/game/vertex/sprite_vertex.glsl");
    m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../src/game/fragment/sprite_fragment.glsl");

    m_shaders.add_uniform_attr("tex");
    m_shaders.add_uniform_attr("mapData");
    m_shaders.add_uniform_attr("scale");
    m_shaders.add_uniform_attr("camera");
    m_shaders.add_uniform_attr("screenResolution");
    m_shaders.add_uniform_attr("zoom");
    m_shaders.link();
}

void graphic::sprite_mesh::free() {
}



void graphic::sprite_mesh::update(const graphic::settings& settings) {
    m_shaders.set_uniform_attr<int>("tex", 0);
    m_shaders.set_uniform_attr<int>("mapData", 1);
    m_shaders.set_uniform_attr<float>("scale", 0.025, 0.025);// * ((double) m_texture->height() / m_texture->width()));
    m_shaders.set_uniform_attr<float>("zoom", settings.zoomX, settings.zoomY);
    m_shaders.set_uniform_attr<float>("screenResolution", settings.currentWidth, settings.currentHeight);
    m_shaders.set_uniform_attr<float>("camera", settings.cameraX, settings.cameraY);
}


void graphic::sprite_mesh::build_vertex() {
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

void graphic::sprite_mesh::build_uv() {
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

void graphic::sprite_mesh::build_position() {
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

void graphic::sprite_mesh::build_type() {
    set_vertex_attr_dimension(3, 1);

    for (size_t i = 0; i < m_objs.size(); i++) {
        add_vertex_attr(3, (float)m_objs[i].type);
        add_vertex_attr(3, (float)m_objs[i].type);
        add_vertex_attr(3, (float)m_objs[i].type);
        add_vertex_attr(3, (float)m_objs[i].type);
        add_vertex_attr(3, (float)m_objs[i].type);
        add_vertex_attr(3, (float)m_objs[i].type);
    }
}




