#pragma once

#include <cstdint>
#include <glad/glad.h>
#include "err.h"
#include "VertexBuffer.h"

struct VertexArray{
    VertexArray();
    ~VertexArray();

    void setLayoutAttributeVec3(VertexBuffer* vbo);
    void bind();
    void unBind();

    unsigned int mId;
    uint32_t mStride;
    uint32_t mAttribIndex;
};
