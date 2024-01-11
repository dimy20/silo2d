#include "IndexBuffer.h"
#include <glad/glad.h>
#include "err.h"
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int * indices, size_t size, VertexArray * vao){
    assert(indices != NULL && size > 0 && vao != NULL);
    vao->bind();

    GL_CALL(glGenBuffers(1, &mId));

    bind();

    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW));

    unBind();

    mCount = size / sizeof(indices[0]);

    vao->unBind();
};

IndexBuffer::~IndexBuffer() { GL_CALL(glDeleteBuffers(1, &mId)); }

void IndexBuffer::bind(){ GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId)); }

void IndexBuffer::unBind(){ GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
