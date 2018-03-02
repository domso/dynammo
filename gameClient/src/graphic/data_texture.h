#pragma once

#include <stdint.h>
#include "src/graphic/base_texture.h"

namespace graphic {
    class data_texture : public base_texture {
    public:
        data_texture(const uint32_t* data, const int width, const int height);
        data_texture(const data_texture& copy) = delete;
        data_texture(data_texture&& move);

        void copy();
    private:
        const uint32_t* m_data;
        int m_width;
        int m_height;
    };
}

