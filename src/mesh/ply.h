#pragma once

/**
 * @file ply.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief load and render ply files
 * @version 0.1
 * @date 2021-07-20
 *
 */
#include <array>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "mesh/mesh.h"
#include "render/camera.h"

namespace glr::mesh {
class PLY : public Mesh {
 public:
  explicit PLY(std::string);
  ~PLY() override = default;

  no_copy(PLY);
  no_move(PLY);

  void Render() override;

 private:
  std::vector<std::array<double, 3>> vertices_;
  std::vector<unsigned int> faces_indices_;
  // std::vector<std::array<double, 3>> normals_;
  // std::vector<std::array<double, 3>> texcoords_;
  // std::vector<std::array<double, 3>> colors_;

  GLuint vao_ = 0;
};
}  // namespace glr::mesh