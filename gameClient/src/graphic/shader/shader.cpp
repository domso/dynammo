#include "shader.h"
#include <sstream>
#include <assert.h>

graphic::shader::shader(const int shaderType, const std::string& filename) {
    m_shaderHandle = create_shader(shaderType, filename);
}

graphic::shader::shader(graphic::shader&& move) {
    m_shaderHandle = move.m_shaderHandle;
    move.m_shaderHandle = 0;
}

graphic::shader::~shader() {
    if (m_shaderHandle != 0) {
        glDeleteShader(m_shaderHandle);
    }
}

bool graphic::shader::valid() const {
    return m_shaderHandle != 0;
}

GLuint graphic::shader::internal_handle() {
    return m_shaderHandle;
}

std::vector<char> graphic::shader::load_shader_file(const std::string& filename) {
    std::ifstream t(filename);
    std::vector<char> result;

    if (t.is_open()) {
        std::stringstream buffer;
        buffer << t.rdbuf();

        while (buffer.good()) {
            result.push_back(buffer.get());
        }

        result[result.size() - 1] = 0;
    }

    return result;
}

GLuint graphic::shader::create_shader(const int shaderType, const std::string& filename) {
    std::vector<char> shaderText = load_shader_file(filename);

    if (shaderText.size() == 0) {
        std::cout << "Could not open " << filename << std::endl;
        return 0;
    }   
    
    int status;
    char* src = shaderText.data();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> logText(logLength + 1, ' ');
        glGetShaderInfoLog(shader, logText.size(), nullptr, logText.data());
        logText[logText.size() - 1] = 0;

        std::cout << "Error while loading '" << filename << "' " << logText.data();

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}


