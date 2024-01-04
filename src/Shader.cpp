#include <fstream>
#include <optional>
#include <string>
#include <glad/glad.h>
#include <iostream>

#include "Shader.h"
#include "err.h"

void Shader::use() { GL_CALL(glUseProgram(mId)); }

void Shader::setMat4(const char *name, const glm::mat4& m){
    GL_CALL(glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_FALSE, glm::value_ptr(m)));
}

void Shader::setInt(const char *name, int value){
    GL_CALL(glUniform1i(glGetUniformLocation(mId, name), value));
}

void Shader::setBool(const char *name, bool value){ setInt(name, value); }

void Shader::setFloat(const char *name, float value){
    GL_CALL(glUniform1f(glGetUniformLocation(mId, name), value));
}

void Shader::setVec3(const char *name, const glm::vec3& v){
    GL_CALL(glUniform3f(glGetUniformLocation(mId, name), v.x, v.y, v.z));
}

bool Shader::compile(uint32_t shaderId, const char *src){
    GL_CALL(glShaderSource(shaderId, 1, &src, NULL));
    GL_CALL(glCompileShader(shaderId));

    int ok;
    char info_log[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ok);

    if(!ok){
        glGetShaderInfoLog(shaderId, 512, NULL, info_log);
        LOG_ERR_FMT("Shader compilation failed: %s\n", info_log);
        return false;
    }

    return ok;
}

bool read_file_to_string(const char *filename, std::string& s_buf){
    std::ifstream f(filename);

    if(!f.is_open()){
        return false;
    }

    f.seekg(0, std::ios::end);
    long size = f.tellg();
    f.seekg(0, std::ios::beg);

    s_buf.resize(static_cast<size_t>(size));
    f.read(&s_buf[0], size);

    return true;
};

bool Shader::load(const char *vertexPath, const char *fragPath){
    std::string vertexSrc, fragSrc;

    if(!read_file_to_string(vertexPath, vertexSrc) ||
            !read_file_to_string(fragPath, fragSrc)){
        return false;
    }

    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    if(vertexShader == 0 || fragShader == 0){
        return false;
    }

    if(!compile(vertexShader, vertexSrc.c_str()) || !compile(fragShader, fragSrc.c_str())){
        return false;
    }

    uint32_t shaderProgram = glCreateProgram();

    GL_CALL(glAttachShader(shaderProgram, vertexShader));
    GL_CALL(glAttachShader(shaderProgram, fragShader));
    GL_CALL(glLinkProgram(shaderProgram));

    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragShader));

    char log[512];
    int ok;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok);

    if(!ok) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        LOG_ERR_FMT("Failed to compile shader program: %s\n", log);
        return false;
    }

    mId = shaderProgram;
    return true;
};
