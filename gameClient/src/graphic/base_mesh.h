#ifndef gameClient_graphic_base_mesh_h
#define gameClient_graphic_base_mesh_h

#include <epoxy/gl.h>
#include <vector>
#include <string>
#include <functional>
#include "shader_program.h"
#include "base_texture.h"

namespace graphic {
    class base_mesh {
    public:
        base_mesh();
        base_mesh(const base_mesh& copy) = delete;
        base_mesh(base_mesh&& move) = delete;
        ~base_mesh();
                        
        virtual void load() = 0;
        virtual void free() = 0;
        virtual void update() = 0;
        
        void add_vertex_attr(const int attrNr, const float value);
        void set_vertex_attr(const int attrNr, const int vertexNr, const float value);
        void set_vertex_attr_dimension(const int attrNr, const int dim);
                
        void add_texture(base_texture& tex);
                
        void realize();
        void unrealize();
        void render();
        
    protected:
        shader_program m_shaders;
    private:               
        std::vector<std::vector<GLfloat>> m_internalVertexBuffer;
        std::vector<GLuint> m_externalVertexBuffer;
        std::vector<GLuint> m_externalVertexGroup;
        std::vector<int> m_externalVertexAttrDimension;
        
        
        std::vector<base_texture*> m_textures;
    };
}

#endif