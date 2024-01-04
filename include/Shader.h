#pragma once
#include <string>
#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Shader{

    bool load(const char *vertex_shader_path, const char *frag_shader_path);
    void use();

    void setMat4(const char *name, const glm::mat4& m);
    void setInt(const char *name, int value);
    void setBool(const char *name, bool value);

    void setFloat(const char *name, float value);
    void setVec3(const char *name, const glm::vec3& v);

    private:
        bool compile(uint32_t shaderId, const char * src);
        uint32_t mId;
};
