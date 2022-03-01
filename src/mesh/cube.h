#pragma once

#include <array>
#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "glfw/deps/linmath.h"
#include "mesh/mesh.h"

namespace glr::mesh {
class Cube : public Mesh {
 private:
  // vertex
  static constexpr float vertices_data[8 * 3] = {-0.3F, -0.3F, -0.3F, 0.3F,  -0.3F, -0.3F, 0.3F, 0.3F,
                                                 -0.3F, -0.3F, 0.3F,  -0.3F, -0.3F, -0.3F, 0.3F, 0.3F,
                                                 -0.3F, 0.3F,  0.3F,  0.3F,  0.3F,  -0.3F, 0.3F, 0.3F};
  static constexpr int indices_data[6 * 2 * 3] = {0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7,
                                                  4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1};

  // vertex shader
  static constexpr auto vertex_shader_text =
      "#version 410\n"
      "uniform mat4 MVP;\n"
      "in vec3 Color;\n"
      "in vec3 Position;\n"
      "out vec3 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    gl_Position = MVP * vec4(Position, 1.0);\n"
      "    Frag_Color = Color;\n"
      "}\n";

  // fragment shader
  static constexpr auto fragment_shader_text =
      "#version 410\n"
      "in vec3 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    gl_FragColor = vec4(Frag_Color, 1.0);\n"
      "}\n";

 public:
  explicit Cube(GLFWwindow* window) : window_(window) {
    // vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    GLuint element_buffer{0};
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices_data, GL_STATIC_DRAW);

    // vertex buffer
    GLuint vertex_buffer{0};
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices_data, GL_STATIC_DRAW);

    // vertest shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);

    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);

    program_ = glCreateProgram();
    glAttachShader(program_, vertex_shader);
    glAttachShader(program_, fragment_shader);
    glLinkProgram(program_);

    mvp_location_ = glGetUniformLocation(program_, "MVP");
    GLuint vpos_location = glGetAttribLocation(program_, "Position");
    GLuint vcol_location = glGetAttribLocation(program_, "Color");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  }

  void Render() override {
    mat4x4 m;
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, static_cast<float>(glfwGetTime()));
    mat4x4_rotate_X(m, m, static_cast<float>(glfwGetTime()));
    mat4x4_rotate_Y(m, m, static_cast<float>(glfwGetTime()));

    int height = 0, width = 0;
    glfwGetFramebufferSize(window_, &width, &height);
    float ratio = width / static_cast<float>(height);

    mat4x4 p;
    mat4x4_ortho(p, -ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

    mat4x4 mvp;
    mat4x4_mul(mvp, p, m);

    glUseProgram(program_);
    glBindVertexArray(vao_);
    glUniformMatrix4fv(mvp_location_, 1, GL_FALSE, (const GLfloat*)mvp);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }

  ~Cube() override = default;
  no_copy(Cube);
  no_move(Cube);

 private:
  GLFWwindow* window_ = nullptr;
  GLuint vao_ = 0;
  GLuint ebo_ = 0;
  GLuint program_ = 0;
  GLuint mvp_location_ = 0;
};
}  // namespace glr::mesh