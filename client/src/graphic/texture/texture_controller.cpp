#include "src/graphic/texture/texture_controller.h"

void graphic::texture_controller::register_texture(
    const std::string& key,
    const std::shared_ptr<graphic::base_texture>& texture
) {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_textures[key] = texture;
}

std::optional<std::shared_ptr<graphic::base_texture>> graphic::texture_controller::get_texture(const std::string& key) {
    std::lock_guard<std::mutex> lg(m_mutex);

    auto it = m_textures.find(key);
    if (it != m_textures.end()) {
        return it->second;
    }
    
    return std::nullopt;
}

void graphic::texture_controller::clean_unused() {
    std::lock_guard<std::mutex> lg(m_mutex);
    for (auto& it : m_textures) {
        if (it.second.use_count() == 1) {
            m_textures.erase(it.first);
        }
    }    
}

void graphic::texture_controller::clear() {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_textures.clear();
}

