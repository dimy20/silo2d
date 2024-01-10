#include "VertexBuffer.h"
#include <glad/glad.h>
#include "err.h"

VertexBuffer::VertexBuffer(const void * data, size_t size, uint32_t count, GLenum usage){
    GL_CALL(glGenBuffers(1, &mId));
    bind(); GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage)); unBind();
    mCount = count;
}

VertexBuffer::~VertexBuffer() { GL_CALL(glDeleteBuffers(1, &mId)); }

void VertexBuffer::bind() { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mId)); }

void VertexBuffer::unBind() { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void VertexBuffer::updateData(uint32_t offset, uint32_t size, const void *data){
    bind(); GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data)); unBind();
};

void VertexBuffer::setData(const void *data, unsigned int size, uint32_t usage){
    clear();
    bind(); GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage)); unBind();
};

void VertexBuffer::clear() {
    bind(); GL_CALL(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW)); unBind();
};
