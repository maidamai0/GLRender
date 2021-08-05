#include "mesh/ply.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <vector>

#include "common/log.h"
#include "common/singleton.h"
#include "common/swtich.h"
#include "common/use_busy_dialog.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "happly/happly.h"
#include "spdlog/fmt/bundled/format.h"
#include "spdlog/stopwatch.h"

namespace glr::mesh {
PLY::PLY(std::string path) : Mesh(std::move(path)) {
  std::thread(&PLY::load_from_file, this, std::move(path)).detach();
}

void PLY::Render() {
  if (should_create_buffer_) {
    create_vertex_buffer();
  }
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, faces_indices_.size(), GL_UNSIGNED_INT, nullptr);
}

void PLY::load_from_file(std::string&& path) {
  common::use_busy_dialog busy;
  file_ = std::make_unique<happly::PLYData>(file_path_);
  should_create_buffer_ = true;
}

void PLY::create_vertex_buffer() {
  vertices_ = file_->getVertexPositions();
  for (const auto& v : vertices_) {
    bound_.x.min = std::min(bound_.x.min, v[0]);
    bound_.x.max = std::max(bound_.x.max, v[0]);

    bound_.y.min = std::min(bound_.y.min, v[1]);
    bound_.y.max = std::max(bound_.y.max, v[1]);

    bound_.z.min = std::min(bound_.z.min, v[2]);
    bound_.z.max = std::max(bound_.z.max, v[2]);
  }

  LOGI("x [{}, {}]", bound_.x.min, bound_.x.max);
  LOGI("y [{}, {}]", bound_.y.min, bound_.y.max);
  LOGI("z [{}, {}]", bound_.z.min, bound_.z.max);
  LOGI("origin is [{}, {}, {}]", bound_.x.middle(), bound_.y.middle(), bound_.z.middle());

  for (const auto f : file_->getFaceIndices<GLuint>()) {
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
  should_create_buffer_ = false;
}

}  // namespace glr::mesh