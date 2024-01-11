
#pragma once

#include <stdio.h>
#include "VertexArray.h"

struct IndexBuffer{
    IndexBuffer(const unsigned int * indices, size_t size, VertexArray * vao);
    ~IndexBuffer();

    void bind();
    void unBind();

        size_t mCount;
    private:
        unsigned int mId;
};
