#include "shader.h"
#include <sstream>

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
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::vector<char> result;

    while (buffer.good()) {
        result.push_back(buffer.get());
    }
    result[result.size() - 1] = 0;
    
    return result;
}

GLuint graphic::shader::create_shader(const int shaderType, const std::string& filename) {
    GLuint shader = glCreateShader(shaderType);
    std::vector<char> shaderText = load_shader_file(filename);
    int status;
    char* src = shaderText.data();
   
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


