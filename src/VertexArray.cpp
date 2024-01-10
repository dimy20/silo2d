#include "VertexArray.h"
#include <cstdint>

VertexArray::VertexArray() {
    GL_CALL(glGenVertexArrays(1, &mId)); 
    mStride = 0;
    mAttribIndex = 0;
}

VertexArray::~VertexArray(){ GL_CALL(glDeleteVertexArrays(1, &mId)); }

void VertexArray::setLayoutAttributeVec3(VertexBuffer* vbo) {
    bind();
    vbo->bind();
    uintptr_t offset = static_cast<uintptr_t>(mStride);
    mStride += sizeof(float) * 3;

    GL_CALL(glVertexAttribPointer(mAttribIndex,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  mStride,
                                  (const void *)(offset)));
    GL_CALL(glEnableVertexAttribArray(mAttribIndex));
    mAttribIndex++;
    unBind();
    vbo->unBind();
}

void VertexArray::bind(){ GL_CALL(glBindVertexArray(mId)); }

void VertexArray::unBind() { GL_CALL(glBindVertexArray(0)); }
