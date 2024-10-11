#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "assets.hpp"
#include "debug/opengl_debug.hpp"

/*
constexpr int WINDOW_HEIGHT{800};
constexpr int WINDOW_WIDTH{800};

class Examples {
public:
  static uint32_t create_triangle() {
    float vertex_data[]{
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f,
    };
    uint32_t vertex_buffer{};
    uint32_t vertex_array{};
    glCreateVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glCreateBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return vertex_array;
  }

  static uint32_t create_square() {
    float vertex_data[]{-0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
    int n_triangles{2};
    int n_vertices_per_triangle{3};
    int n_coordinates_per_vertex{2};

    uint32_t index_data[]{0, 1, 2, 2, 3, 0};

    uint32_t vertex_array{};
    uint32_t vertex_buffer{};
    uint32_t index_buffer{};
    glCreateVertexArrays(1, &vertex_array);
    glCreateBuffers(1, &vertex_buffer);
    glCreateBuffers(1, &index_buffer);

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * n_triangles * n_vertices_per_triangle *
                     n_coordinates_per_vertex,
                 vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(uint32_t) * n_triangles * n_vertices_per_triangle,
                 index_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false,
                          sizeof(float) * n_coordinates_per_vertex, nullptr);

    glEnableVertexAttribArray(0);

    return vertex_array;
  }

  static void event_loop_square(GLFWwindow *window) {
    shader s{SHADER_DIR "vertex.glsl", SHADER_DIR "fragment.glsl"};
    uint32_t square_vertex_array{Examples::create_square()};
    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      s.bind();
      glBindVertexArray(square_vertex_array);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
      }
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  static void event_loop_triangle(GLFWwindow *window) {
    shader s{SHADER_DIR "vertex.glsl", SHADER_DIR "fragment.glsl"};
    uint32_t triangle_vertex_array{Examples::create_triangle()};
    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      s.bind();
      glBindVertexArray(triangle_vertex_array);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
      }
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  static void configure_texture_wrapping() {
    GLenum wrapping_option{GL_REPEAT};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapping_option);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping_option);
    if (wrapping_option == GL_CLAMP_TO_BORDER) {
      float red[]{1.0, 0.0, 0.0, 0.0};
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, red);
    }
  }

  static void configure_texture_filtering() {
    GLenum smooth_appearence{GL_LINEAR};
    GLenum _8bit_appearence{GL_NEAREST};
    GLenum smooth_appearence_also_for_mipmap{GL_LINEAR_MIPMAP_LINEAR};
    GLenum smooth_appearence_but_8bit_appearence_for_mipmap{
        GL_LINEAR_MIPMAP_NEAREST};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    smooth_appearence_also_for_mipmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth_appearence);

    _8bit_appearence;
    smooth_appearence_but_8bit_appearence_for_mipmap;
  }

  static uint32_t texture() {
    configure_texture_wrapping();
    int width{}, height{}, number_of_channels{};
    unsigned char *data{stbi_load(TEXTURE_DIR "wall.jpg", &width, &height,
                                  &number_of_channels, 0)};
    if (data == nullptr) {
      std::cerr << "Failed to load image.\n";
      return 0;
    }
    uint32_t texture{};
    int texture_slot{0};
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + texture_slot);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, static_cast<void *>(data));
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
  }

  struct vao_and_texture {
    uint32_t vao;
    uint32_t texture;
  };

  static vao_and_texture triangle_with_brickwall_texture() {
    int n_position_coordinates_per_vertex{2};
    int n_texture_coordinates_per_vertex{2};

    float bottom_left_vertex[] = {-0.5f, -0.5f, 0.0f, 0.0f};
    float top_center_vertex[] = {0.0f, 0.5f, 0.5f, 1.0f};
    float bottom_right_vertex[] = {0.5f, -0.5f, 1.0f, 1.0f};

    float vertices[]{
        bottom_left_vertex[0],  bottom_left_vertex[1],  bottom_left_vertex[2],
        bottom_left_vertex[3],  top_center_vertex[0],   top_center_vertex[1],
        top_center_vertex[2],   top_center_vertex[3],   bottom_right_vertex[0],
        bottom_right_vertex[1], bottom_right_vertex[2], bottom_right_vertex[3],
    };

    uint32_t vertex_buffer{};
    uint32_t vertex_array{};
    glCreateVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glCreateBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 static_cast<void *>(vertices), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * (n_position_coordinates_per_vertex +
                                           n_texture_coordinates_per_vertex),
                          nullptr);
    uint32_t _texture = texture();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * (n_position_coordinates_per_vertex +
                                           n_texture_coordinates_per_vertex),
                          (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return {.vao = vertex_array, .texture = _texture};
  }

  static void event_loop_triangle_with_brickwall_texture(GLFWwindow *window) {
    shader s{SHADER_DIR "vertex-texture.glsl",
             SHADER_DIR "fragment-texture.glsl"};
    auto res{Examples::triangle_with_brickwall_texture()};
    uint32_t triangle_vertex_array{res.vao};
    uint32_t brickwall_texture{res.texture};
    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glBindTexture(GL_TEXTURE_2D, brickwall_texture);
      s.bind();
      glBindVertexArray(triangle_vertex_array);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
      }
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }
};

void teardown() { glfwTerminate(); }

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  std::cout << "resize window to " << width << ", " << height << ".\n";
  glViewport(0, 0, width, height);
}

void initGLFW() {
  if (glfwInit() == GLFW_FALSE) {
    std::cerr << "Failed to initialize GLFW\n";
    teardown();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
  GLFWwindow *window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL",
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
*/

int main() {
  /*
  GLFWwindow *window{init(false)};
  Examples::event_loop_triangle_with_brickwall_texture(window);

  teardown();
  */
  return EXIT_SUCCESS;
}
