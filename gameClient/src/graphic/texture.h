#ifndef gameClient_graphic_texture_h
#define gameClient_graphic_texture_h

#include <string>
#include <epoxy/gl.h>
#include <gtkmm/image.h>

        
        
namespace graphic {
    class texture {
    public:
        texture(const std::string& filename);
        texture(const texture& copy) = delete;
        texture(texture&& move);        
        ~texture();
        
        void link();
        void bind();
    private:
        bool m_valid;
        Gtk::Image m_image;
        GLuint m_textureHandle;
    };
}

#endif
