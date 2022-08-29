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
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "happly/happly.h"
#include "spdlog/fmt/bundled/format.h"
#include "spdlog/stopwatch.h"

namespace {

/**
 * @brief convert an std::array to glm::vec
 *
 * @tparam ArrayType  value type of std::array
 * @tparam GLMType    value type of glm::vec
 * @tparam size       size of std::array
 * @param array       std::array
 * @return glm::vec   glm::vec with the same size of array
 */
template <typename GLMType, typename ArrayType, size_t size>
auto array_to_glm(const std::array<ArrayType, size>& array) {
  glm::vec<size, float, glm::defaultp> ret{};
  for (size_t i = 0; i < size; ++i) {
    ret[i] = static_cast<GLMType>(array.at(i));
  }

  return ret;
}
}  // namespace

namespace glr::mesh {
PLY::PLY(std::string path) : Mesh(std::move(path)) {
  std::thread(&PLY::load_from_file, this, std::move(path)).detach();
}

PLY::~PLY() = default;

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
  for (const auto& vertex : vertices_) {
    bound_.x.min = std::min(bound_.x.min, vertex[0]);
    bound_.x.max = std::max(bound_.x.max, vertex[0]);

    bound_.y.min = std::min(bound_.y.min, vertex[1]);
    bound_.y.max = std::max(bound_.y.max, vertex[1]);

    bound_.z.min = std::min(bound_.z.min, vertex[2]);
    bound_.z.max = std::max(bound_.z.max, vertex[2]);
  }

  LOGI("x [{}, {}]", bound_.x.min, bound_.x.max);
  LOGI("y [{}, {}]", bound_.y.min, bound_.y.max);
  LOGI("z [{}, {}]", bound_.z.min, bound_.z.max);
  LOGI("center is [{}, {}, {}]", bound_.x.middle(), bound_.y.middle(), bound_.z.middle());

  normals_.resize(vertices_.size());
  for (const auto face : file_->getFaceIndices<GLuint>()) {
    glm::vec3 vert_0{array_to_glm<float>(vertices_[face[0]])};
    glm::vec3 vert_1{array_to_glm<float>(vertices_[face[1]])};
    glm::vec3 vert_2{array_to_glm<float>(vertices_[face[2]])};

    glm::vec3 edge_ab = vert_1 - vert_0;
    glm::vec3 edge_bc = vert_2 - vert_1;
    const auto normal = glm::cross(edge_ab, edge_bc);
    normals_[face[0]] += normal;
    normals_[face[1]] += normal;
    normals_[face[2]] += normal;

    for (const auto& index : face) {
      faces_indices_.push_back(index);
    }
  }

  LOGI("number of vertices is {}", vertices_.size());
  LOGI("size of of vertice data is {}", sizeof(vertices_[0]) * vertices_.size());
  LOGI("number of faces is {}", faces_indices_.size() / 3);
  LOGI("size of of faces data is {}", faces_indices_.size());

  for (auto& normal : normals_) {
    normal = glm::normalize(normal);
  }

  // vao
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // vertex buffer
  GLuint vertex_buffer_positions{0};
  glGenBuffers(1, &vertex_buffer_positions);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_positions);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_[0]) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), nullptr);
  glEnableVertexAttribArray(0);

  GLuint vertex_buffer_normals{0};
  glGenBuffers(1, &vertex_buffer_normals);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_normals);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normals_[0]) * normals_.size(), normals_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);

  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * faces_indices_.size(), faces_indices_.data(), GL_STATIC_DRAW);
  should_create_buffer_ = false;
}

}  // namespace glr::mesh