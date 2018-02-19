#ifndef gameClient_graphic_shader_program_h
#define gameClient_graphic_shader_program_h

#include <epoxy/gl.h>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include "shader.h"

namespace graphic {
    class shader_program {
    public:
        shader_program();
        shader_program(const shader_program& copy) = delete;
        shader_program(shader_program&& move) = delete;
        ~shader_program();

        typedef int shader_type;
        struct shader_types {
            constexpr static const shader_type vertex = GL_VERTEX_SHADER;
            constexpr static const shader_type fragment = GL_FRAGMENT_SHADER;
        };

        template <shader_type T>
        bool add_shader(const std::string& filename) {
            shader newShader(T, filename);

            if (newShader.valid()) {
                m_shaders.push_back(std::move(newShader));
                return true;
            }

            return false;
        }
        
        template <typename T>
        bool set_uniform_attr(const std::string& attr, const T x) {
            return set_uniform_attr_array<T>(attr, &x, 1);
        }
        
        template <typename T>
        bool set_uniform_attr(const std::string& attr, const T x, const T y) {
            T buffer[2];
            buffer[0] = x;
            buffer[1] = y;
            return set_uniform_attr_array<T>(attr, buffer, 2);
        }
        
        template <typename T>
        bool set_uniform_attr(const std::string& attr, const T x, const T y, const T z) {
            T buffer[3];
            buffer[0] = x;
            buffer[1] = y;
            buffer[2] = z;
            return set_uniform_attr_array<T>(attr, buffer, 3);
        }       
        
        template <typename T>
        typename std::enable_if<std::is_same<int, T>::value,bool>::type set_uniform_attr_array(const std::string& attr,const T* x, const int n) {
            if (m_uniformMap.count(attr) > 0) {
                enable();
                glUniform1iv(m_uniformMap[attr], n, x);
                disable();
                
                return true;
            }
            
            return false;
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<float, T>::value,bool>::type set_uniform_attr_array(const std::string& attr,const T* x, const int n) {
            if (m_uniformMap.count(attr) > 0) {
                enable();
                glUniform1fv(m_uniformMap[attr], n,  x);
                disable();
                
                return true;
            }
            
            return false;
        }
        
        void add_uniform_attr(const std::string& attr);
        
        void enable();
        void disable();

        bool link();
    private:
        bool no_link_error();
        void close();
                
        void link_uniform_attr();
        
        GLuint m_program;
        std::vector<shader> m_shaders;
        std::unordered_map<std::string, GLuint> m_uniformMap;
    };
}
#endif

