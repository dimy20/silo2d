#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <stdio.h>

struct VertexBuffer{
    VertexBuffer(const void * data, size_t size, uint32_t count, GLenum usage = GL_STATIC_DRAW);
    ~VertexBuffer();

    void updateData(uint32_t offset, uint32_t size, const void *data);
    /* Sets new data of size <size> in the vertex buffer object.
         * If the VBO contains any data when this function is called, it will be cleared
         * prior to setting the new data.
         * */
    void setData(const void *data, unsigned int size, uint32_t usage = GL_STATIC_DRAW);

    /*Clears all the data contained in an opengl Vertex Buffer Object.
         *It doesnt delete the Vertex Buffer Object itself, it only deallocates the memory
         previously stored int the VBO's.*/

    void clear();

    void bind();
    void unBind();

    unsigned int mId;
    uint32_t mCount;
};
