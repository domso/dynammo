#include "shader_program.h"

graphic::shader_program::shader_program() {

}

graphic::shader_program::~shader_program() {
    close();
}

bool graphic::shader_program::link() {
    bool result = false;
    close();

    m_program = glCreateProgram();

    for (shader& s : m_shaders) {
        glAttachShader(m_program, s.internal_handle());
    }

    glLinkProgram(m_program);

    if (no_link_error()) {
        result = true;
        link_uniform_attr();
    }

    for (shader& s : m_shaders) {
        glDetachShader(m_program, s.internal_handle());
    }
    
    m_shaders.clear();

    return result;
}

void graphic::shader_program::add_uniform_attr(const std::string& attr) {
    m_uniformMap.insert_or_assign(attr, 0);
}

void graphic::shader_program::enable() {
    glUseProgram(m_program);
}

void graphic::shader_program::disable() {
    glUseProgram(0);
}

bool graphic::shader_program::no_link_error() {
    int status;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        int logLength;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> logText(logLength + 1, ' ');
        glGetProgramInfoLog(m_program, logText.size(), nullptr, logText.data());
        logText[logText.size() - 1] = 0;

        std::cerr << "Linking failure: " << logText.data() << std::endl;

        return false;
    }

    return true;
}

void graphic::shader_program::close() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void graphic::shader_program::link_uniform_attr() {
    for (auto& attr : m_uniformMap) {
        attr.second = glGetUniformLocation(m_program, attr.first.c_str());
    }
}
