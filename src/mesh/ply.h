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
#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include "mesh/mesh.h"
#include "render/camera.h"

namespace happly {
class PLYData;
}

namespace glr::mesh {

class PLY : public Mesh {
 public:
  explicit PLY(std::string);
  ~PLY() override = default;

  no_copy(PLY);
  no_move(PLY);

  void Render() override;

 private:
  void load_from_file(std::string&& path);
  void create_vertex_buffer();

  std::vector<std::array<double, 3>> vertices_;
  std::vector<unsigned int> faces_indices_;
  std::vector<glm::vec3> normals_;

  GLuint vao_ = 0;

  std::atomic_bool should_create_buffer_{false};
  std::unique_ptr<happly::PLYData> file_;
};
}  // namespace glr::mesh