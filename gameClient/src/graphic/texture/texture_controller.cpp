#include "src/graphic/texture/texture_controller.h"

std::shared_ptr<graphic::img_texture> graphic::texture_controller::load_img_texture(const std::string& filename) {
    if (m_imgTextures.count(filename) == 0) {
        auto result = m_imgTextures.emplace(std::make_pair(filename, std::make_shared<graphic::img_texture>(filename)));
        return (result.first)->second;
    }

    return m_imgTextures[filename];
}

std::shared_ptr<graphic::img_block_texture> graphic::texture_controller::load_img_block_texture(const std::string& filename, const int first, const int last, const int dimension) {
    if (m_imgBlockTextures.count(filename) == 0) {
        auto result = m_imgBlockTextures.emplace(std::make_pair(filename, std::make_shared<graphic::img_block_texture>(filename, first, last, dimension)));
        return (result.first)->second;
    }

    return m_imgBlockTextures.at(filename);
}

std::shared_ptr<graphic::data_texture> graphic::texture_controller::load_data_texture(const uint32_t* data, const int width, const int height) {
    intptr_t index = (intptr_t) data;
    if (m_dataTextures.count(index) == 0) {
        auto result = m_dataTextures.emplace(std::make_pair(index, std::make_shared<graphic::data_texture>(data, width, height)));
        return (result.first)->second;
    }

    return m_dataTextures[index];
}

void graphic::texture_controller::clean_unused() {
    for (auto& it : m_imgTextures) {
        if (it.second.use_count() == 1) {
            m_imgTextures.erase(it.first);
        }
    }
    
    for (auto& it : m_dataTextures) {
        if (it.second.use_count() == 1) {
            m_dataTextures.erase(it.first);
        }
    }
}

void graphic::texture_controller::clear() {
    m_imgTextures.clear();
    m_dataTextures.clear();
}

