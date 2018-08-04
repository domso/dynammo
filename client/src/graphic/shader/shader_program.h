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
        typename std::enable_if<std::is_same<int, T>::value,void>::type set_uniform_attr(const std::string& attr, const T x) {
            glUniform1i(m_uniformMap[attr], x);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<float, T>::value,void>::type set_uniform_attr(const std::string& attr, const T x) {
            glUniform1f(m_uniformMap[attr], x);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<int, T>::value,void>::type set_uniform_attr(const std::string& attr, const T x, const T y) {
            glUniform2i(m_uniformMap[attr], x, y);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<float, T>::value,void>::type set_uniform_attr(const std::string& attr, const T x, const T y) {
            glUniform2f(m_uniformMap[attr], x, y);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<int, T>::value,void>::type set_uniform_attr(const std::string& attr, const T x, const T y, const T z) {
            glUniform3i(m_uniformMap[attr], x, y, z);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<float, T>::value,void>::type set_uniform_attr(const std::string& attr, const T x, const T y, const T z) {
            glUniform3f(m_uniformMap[attr], x, y, z);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<int, T>::value,void>::type set_uniform_attr_array(const std::string& attr,const T* x, const int n) {
            glUniform1iv(m_uniformMap[attr], n, x);
        }
        
        template <typename T>
        typename std::enable_if<std::is_same<float, T>::value,void>::type set_uniform_attr_array(const std::string& attr,const T* x, const int n) {
            glUniform1fv(m_uniformMap[attr], n,  x);
        }
        
        void add_uniform_attr(const std::string& attr);
        
        void enable();
        void disable();

        bool link();
        void close();
    private:
        bool no_link_error();                
        void link_uniform_attr();
        
        GLuint m_program;
        std::vector<shader> m_shaders;
        std::unordered_map<std::string, GLuint> m_uniformMap;
    };
}
#endif

