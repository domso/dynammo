#ifndef gameClient_graphic_shader_h
#define gameClient_graphic_shader_h

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <epoxy/gl.h>

namespace graphic {
    class shader {
    public:
        shader(const int shaderType, const std::string& filename);
        shader(const shader& copy) = delete;
        shader(shader&& move);
        ~shader();
        
        bool valid() const;
        GLuint internal_handle();
    private:
        std::vector<char> load_shader_file(const std::string& filename);
        GLuint create_shader(const int shaderType, const std::string& filename);

        GLuint m_shaderHandle;
    };
}
#endif
