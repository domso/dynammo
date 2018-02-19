#ifndef gameClient_graphic_mesh_h
#define gameClient_graphic_mesh_h

#include <epoxy/gl.h>
#include <vector>
#include "shader_program.h"
#include "texture.h"

namespace graphic {
    class mesh {
    public:
        mesh();
        mesh(const mesh& copy) = delete;
        mesh(mesh&& move);
        ~mesh();

        void set_vertex_attr(const int attrNr, const float value);
        void set_vertex_attr_dimension(const int attrNr, const int dim);
                
        void set_shader_program(shader_program* prog);
        void add_texture(texture* tex);
        
        void realize();
        void unrealize();

        void render();
    private:            
        
        std::vector<std::vector<GLfloat>> m_internalVertexBuffer;
        std::vector<GLuint> m_externalVertexBuffer;
        std::vector<GLuint> m_externalVertexGroup;
        std::vector<int> m_externalVertexAttrDimension;
                
        shader_program* m_shaders;
        std::vector<texture*> m_textures;
    };
}

#endif
