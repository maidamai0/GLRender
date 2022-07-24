#pragma once

#include "glad/glad.h"

#include <array>
#include "GLFW/glfw3.h"
#include "glfw/deps/linmath.h"
#include "mesh/mesh.h"

namespace glr::mesh {
class Triangle : public Mesh {
 private:
  // clang-format off
const std::array<float, 9> vertices = {
    -0.5F, -0.5F, 0.F,
    0.5F, -0.5F, 0.F,
    0.F, 0.5F, 0.F,
};
  // clang-format on

 public:
  explicit Triangle() {
    // vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // vertex buffer
    GLuint vertex_buffer{0};
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
  }

  void Render() override {
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  ~Triangle() override = default;
  no_copy(Triangle);
  no_move(Triangle);

 private:
  GLuint vao_ = 0;
};
}  // namespace glr::mesh