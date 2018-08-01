#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "src/graphic/texture/img_texture.h"
#include "src/graphic/texture/img_block_texture.h"
#include "src/graphic/texture/data_texture.h"

#include <mutex>
#include <optional>

namespace graphic {
    class texture_controller {
    public:
        void register_texture(const std::string& key, const std::shared_ptr<base_texture>& texture);
        std::optional<std::shared_ptr<base_texture>> get_texture(const std::string& key);
        
        void clean_unused();               
        void clear();
    private:   
        std::mutex m_mutex;
        std::unordered_map<std::string, std::shared_ptr<base_texture>> m_textures;
    };
}
