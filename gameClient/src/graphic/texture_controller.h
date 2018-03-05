#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "src/graphic/img_texture.h"
#include "src/graphic/data_texture.h"

namespace graphic {
    class texture_controller {
    public:
        std::shared_ptr<img_texture> load_img_texture(const std::string& filename);
        // data is used as the identifier! (-> not including the resolution!)
        std::shared_ptr<data_texture> load_data_texture(const uint32_t* data, const int width, const int height);        
        void clean_unused();
    private:   
        std::unordered_map<std::string, std::shared_ptr<img_texture>> m_imgTextures;
        std::unordered_map<intptr_t, std::shared_ptr<data_texture>> m_dataTextures;
    };
}
