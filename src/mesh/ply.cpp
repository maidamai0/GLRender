#include "mesh/ply.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "common/log.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "happly/happly.h"
#include "spdlog/fmt/bundled/format.h"

namespace glr::mesh {
PLY::PLY(std::string path) : Mesh(std::move(path)) {
  happly::PLYData file(file_path_);
  vertices_ = file.getVertexPositions();
  for (const auto f : file.getFaceIndices<GLuint>()) {
    for (const auto& i : f) {
      faces_indices_.push_back(i);
    }
  }

  LOGI("number of vertices is {}", vertices_.size());
  LOGI("size of of vertice data is {}", sizeof(vertices_[0]) * vertices_.size());
  LOGI("number of faces is {}", faces_indices_.size() / 3);
  LOGI("size of of faces data is {}", faces_indices_.size());

  // vao
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // vertex buffer
  GLuint vertex_buffer{0};
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_[0]) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), nullptr);
  glEnableVertexAttribArray(0);

  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * faces_indices_.size(), faces_indices_.data(), GL_STATIC_DRAW);
}

void PLY::Render() {
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, faces_indices_.size(), GL_UNSIGNED_INT, nullptr);
}
}  // namespace glr::mesh