#pragma once

#include <stdio.h>
#include <glad/glad.h>
#include <cassert>

#define LOG(fmt, ...) do {        \
    fprintf(stdout, fmt, ##__VA_ARGS__);  \
    fprintf(stdout, "\n");                \
} while (0)

#define LOG_ERR(msg) fprintf(stderr, "Error: %s\n", msg);

#define LOG_ERR_FMT(fmt, ...) do {        \
    fprintf(stderr, fmt, ##__VA_ARGS__);  \
    fprintf(stderr, "\n");                \
} while (0)

#define OPENGL_ERR_CHECK                             \
({                                                   \
        bool ok = true;                              \
        GLenum err;                                  \
        while((err = glGetError()) != GL_NO_ERROR){  \
            LOG_ERR_FMT("Opengl error: %d", err);    \
            ok = false;                              \
        }                                            \
        ok;                                          \
    })                                               \

inline void gl_clear_error(){
    while(glGetError() != GL_NO_ERROR);
};

inline bool gl_debug_call(const char * func, const char *file, int line){
    GLenum err;
    bool ok = true;
    while((err = glGetError())){
        LOG_ERR_FMT("Opengl error: %d\n at: %s:%d \nfunc: %s", err, file, line, func);
        ok = false;
    };
    return ok;
};

#define GL_CALL(v) gl_clear_error(); \
    v; \
    assert(gl_debug_call(#v, __FILE__, __LINE__))
