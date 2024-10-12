#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <optional>
#include <stb_image.h>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>

#include "assets.hpp"
#include "debug/opengl_debug.hpp"

constexpr int WINDOW_HEIGHT{800};
constexpr int WINDOW_WIDTH{800};

void teardown() { glfwTerminate(); }

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void initGLFW() {
  if (glfwInit() == GLFW_FALSE) {
    std::cerr << "Failed to initialize GLFW\n";
    teardown();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initGLEW() {
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    teardown();
    exit(EXIT_FAILURE);
  }
}

GLFWwindow *create_window() {
  GLFWwindow *window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Checkers3D",
                                      nullptr, nullptr)};
  if (window == nullptr) {
    std::cerr << "Failed to create GLFWwindow\n";
    teardown();
    exit(EXIT_FAILURE);
  }
  return window;
}

GLFWwindow *init(bool wireframe_mode) {
  initGLFW();
  GLFWwindow *window{create_window()};
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  initGLEW();
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(open_gl::debug_callback, 0);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  if (wireframe_mode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  return window;
}

namespace RGBColor {
struct color {
  float r;
  float g;
  float b;
  float a;
};
constexpr color Black{0.0f, 0.0f, 0.0f, 0.0f};
constexpr color Red{1.0f, 0.0f, 0.0f, 0.0f};
} // namespace RGBColor

using Filepath = std::filesystem::path;
template <class T> using Opt = std::optional<T>;

Opt<std::string> getFileContents(const Filepath &path) {
  std::fstream file{path};
  if (!file.is_open()) {
    return {};
  }
  std::stringstream stream{};
  stream << file.rdbuf();
  return stream.str();
}

using Shader = Opt<uint32_t>;

Shader shader(const Filepath &shaderPath, GLuint shaderType) {
  Opt<std::string> shaderSource = getFileContents(shaderPath);
  if (!shaderSource.has_value()) {
    std::cerr << "No shader source\n";
    return {};
  }
  const char *sourceString = shaderSource->c_str();
  uint32_t shaderId = glCreateShader(shaderType);
  glShaderSource(shaderId, 1, &sourceString, nullptr);
  glCompileShader(shaderId);

  int status{};
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
  if (!status) {
    constexpr std::size_t len{1024};
    int32_t logLen{};
    char log[len]{};
    glGetShaderInfoLog(shaderId, len, &logLen, log);
    std::cerr << "shader compilation failed\n";
    std::cerr << log << "\n";
    return {};
  }
  return shaderId;
}

using Program = Opt<uint32_t>; // FIXME: cpp doesn't enforce type alias, so
                               // Program == Shader

Program program(const Shader &vertexShader, const Shader &fragmentShader) {
  uint32_t programId = glCreateProgram();
  if (!vertexShader.has_value() || !fragmentShader.has_value()) {
    std::cerr << "vertex/fragment shader doesn't exist\n";
    return {};
  }
  glAttachShader(programId, *vertexShader);
  glAttachShader(programId, *fragmentShader);
  glLinkProgram(programId);
  int status{};
  glGetProgramiv(programId, GL_LINK_STATUS, &status);
  if (!status) {
    constexpr std::size_t len{1024};
    int32_t logLen{};
    char log[len]{};
    glGetProgramInfoLog(programId, len, &logLen, log);
    std::cerr << "program linking failed\n";
    std::cerr << log << "\n";
    return {};
  }
  return programId;
}

int board() {
  GLFWwindow *window{init(false)};
  float boardVertices[] = {
      -0.5f, -0.5f, 0.0f, // down left
      -0.5f, 0.5f,  0.0f, // up left
      0.5f,  0.5f,  0.0f, // up right
      0.5f,  -0.5f, 0.0f, // down right
      -0.5f, -0.5f, 0.2f, // 4
      -0.5f, 0.5f,  0.2f, // 5
      0.5f,  0.5f,  0.2f, // 6
      0.5f,  -0.5f, 0.2f, // 7
  };
  uint32_t indeces[] = {
      0, 1, 2, 0, 2, 3, // front
      4, 5, 6, 4, 5, 7, // back
      1, 5, 6, 1, 6, 7, // top
      0, 4, 7, 0, 7, 3, // bottom
      0, 1, 4, 1, 5, 4, // left
      3, 2, 7, 2, 6, 7, // right
  };

  uint32_t vertexBuffer{};
  uint32_t indexBuffer{};
  uint32_t vertexArray{};
  glCreateVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);
  glCreateBuffers(1, &vertexBuffer);
  glCreateBuffers(1, &indexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(boardVertices), (void *)boardVertices,
               GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), (void *)indeces,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  Shader vertexShader{shader(SHADER_DIR "vertex.glsl", GL_VERTEX_SHADER)};
  Shader fragmentShader{shader(SHADER_DIR "fragment.glsl", GL_FRAGMENT_SHADER)};
  Program finalProgram{program(vertexShader, fragmentShader)};
  if (!finalProgram.has_value()) {
    std::cerr << "no program\n";
    return EXIT_FAILURE;
  }

  while (!glfwWindowShouldClose(window)) {
    glClearColor(RGBColor::Black.r, RGBColor::Black.g, RGBColor::Black.b,
                 RGBColor::Black.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(*finalProgram);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBindVertexArray(vertexArray);

    glDrawElements(GL_TRIANGLES, sizeof(indeces) / sizeof(indeces[0]),
                   GL_UNSIGNED_INT, nullptr);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  teardown();
  return EXIT_SUCCESS;
}

int main() { return board(); }
