#include "src/game/mesh/layer_mesh.h"


graphic::layer_mesh::layer_mesh(const region::layer<uint32_t>& layer, texture_controller& texCtrl) {
    add_texture(std::make_shared<graphic::img_texture>("../res/newgrass.png"));

    auto layerTex = std::make_shared<graphic::data_texture>(layer.data(), layer.width, layer.height);
    texCtrl.register_texture("data::layer", layerTex);
    add_texture(layerTex);
}

void graphic::layer_mesh::load() {
    build();

    if (get_id() == 0) {
        m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../src/game/vertex/terrain_layer.glsl");
    } else {
        m_shaders.add_shader<graphic::shader_program::shader_types::vertex>("../src/game/vertex/water_layer.glsl");
    }

    m_shaders.add_shader<graphic::shader_program::shader_types::fragment>("../src/game/fragment/fragment.glsl");

    m_shaders.add_uniform_attr("groundTex");
    m_shaders.add_uniform_attr("mapData");
    m_shaders.add_uniform_attr("regionID");
    m_shaders.add_uniform_attr("zoom");
    m_shaders.add_uniform_attr("camera");
    m_shaders.add_uniform_attr("screenResolution");
    m_shaders.add_uniform_attr("waterOffset");

    m_shaders.link();
}

void graphic::layer_mesh::free() {
}

void graphic::layer_mesh::update(const graphic::settings& settings) {
    m_shaders.set_uniform_attr<int>("groundTex", 0);
    m_shaders.set_uniform_attr<int>("mapData", 1);
    m_shaders.set_uniform_attr<int>("regionID", 0);
    m_shaders.set_uniform_attr<float>("zoom", settings.zoomX, settings.zoomY);
    m_shaders.set_uniform_attr<float>("screenResolution", settings.currentWidth, settings.currentHeight);
    m_shaders.set_uniform_attr<float>("camera", settings.cameraX, settings.cameraY);

    m_shaders.set_uniform_attr<float>("waterOffset", waterOffset);
    waterOffset += 1 * settings.frameDuration;

}

void graphic::layer_mesh::build() {
    set_vertex_attr_dimension(0, 3);
    set_vertex_attr_dimension(1, 2);

    for (int y = 0; y < resolution; y++) {
        for (int x = 0; x < resolution; x++) {
            add_vertex_attr(0, x);
            add_vertex_attr(0, y);
            add_vertex_attr(0, 0);
            add_vertex_attr(1, 0);
            add_vertex_attr(1, 0);

            add_vertex_attr(0, x + 1);
            add_vertex_attr(0, y);
            add_vertex_attr(0, 0);
            add_vertex_attr(1, 1);
            add_vertex_attr(1, 0);

            add_vertex_attr(0, x);
            add_vertex_attr(0, y + 1);
            add_vertex_attr(0, 0);
            add_vertex_attr(1, 0);
            add_vertex_attr(1, 1);

            add_vertex_attr(0, x + 1);
            add_vertex_attr(0, y + 1);
            add_vertex_attr(0, 0);
            add_vertex_attr(1, 1);
            add_vertex_attr(1, 1);

            add_vertex_attr(0, x + 1);
            add_vertex_attr(0, y);
            add_vertex_attr(0, 0);
            add_vertex_attr(1, 1);
            add_vertex_attr(1, 0);

            add_vertex_attr(0, x);
            add_vertex_attr(0, y + 1);
            add_vertex_attr(0, 0);
            add_vertex_attr(1, 0);
            add_vertex_attr(1, 1);
        }
    }
}
