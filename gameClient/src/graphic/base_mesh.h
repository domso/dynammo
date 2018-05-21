#ifndef gameClient_graphic_base_mesh_h
#define gameClient_graphic_base_mesh_h

#include <epoxy/gl.h>
#include <vector>
#include <string>
#include <functional>
#include "shader_program.h"
#include "base_texture.h"
#include "src/graphic/settings.h"

namespace graphic {
    class base_mesh {
    public:
        base_mesh();
        base_mesh(const base_mesh& copy) = delete;
        base_mesh(base_mesh&& move) = delete;
        ~base_mesh();
                        
        virtual void load() = 0;
        virtual void free() = 0;
        virtual void update(const graphic::settings& settings) = 0;
                       
        void set_id(const uint32_t id);
        uint32_t get_id() const;
        
        void add_vertex_attr(const int attrNr, const float value);
        void set_vertex_attr(const int attrNr, const int vertexNr, const float value);
        void set_vertex_attr_dimension(const int attrNr, const int dim);
                
        void add_texture(base_texture& tex);
                
        void realize();
        void unrealize();
        void render(const graphic::settings& settings);
        
    protected:
        shader_program m_shaders;
    private:               
        std::vector<std::vector<GLfloat>> m_internalVertexBuffer;
        std::vector<GLuint> m_externalVertexBuffer;
        GLuint m_externalVertexGroup;
        std::vector<int> m_externalVertexAttrDimension;
        
        uint32_t m_id;
        
        std::vector<base_texture*> m_textures;
    };
}

#endif
