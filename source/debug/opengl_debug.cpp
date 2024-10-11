#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include "debug/opengl_debug.hpp"

void open_gl::debug_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
) {
    std::cout << "source: " << source << "\n";
    std::cout << "type: " << type << "\n";
    std::cout << "severity: " << severity << "\n";
    std::cout << "id: " << id << "\n";
    std::cout << "length: " << length << "\n";
    std::cout << "message: " << message << "\n";
}
