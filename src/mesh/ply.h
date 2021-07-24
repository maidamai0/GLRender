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

#include "GLFW/glfw3.h"

#include "mesh/mesh.h"
#include "viewer/camera.h"

namespace glr::mesh {
class PLY : public Mesh {
 public:
  explicit PLY(GLFWwindow* window, std::string);
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

  GLFWwindow* window_ = nullptr;
  GLuint vao_ = 0;
  GLuint program_ = 0;
  GLint projection_location_ = 0;
  GLint view_location_ = 0;
  GLint model_location_ = 0;
  GLint color_location_ = 0;
  Camera camera_{{0.0F, 0.0F, 5.0F}};
};
}  // namespace glr::mesh