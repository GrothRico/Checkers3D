#pragma once

#include <GL/glew.h>

namespace open_gl {
    void debug_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam
    );
}
