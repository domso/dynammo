#ifndef gameClient_graphic_base_texture_h
#define gameClient_graphic_base_texture_h

#include <string>
#include <epoxy/gl.h>
#include <gtkmm/image.h>
#include <mutex>
              
namespace graphic {
    class base_texture {
    public:
        base_texture();
        base_texture(const base_texture& copy) = delete;
        base_texture(base_texture&& move);        
        ~base_texture();
            
        virtual void copy() = 0;
        
        void free();
        void link();
        void bind();
    private:        
        int m_linkCounter;
        std::mutex m_mutex;
        GLuint m_textureHandle;
    };
}

#endif
