#pragma once

#include "mesh/mesh.h"
#include "render/camera.h"

#include "glad/glad.h"
#include "glm/fwd.hpp"

#include <vector>

namespace glr::render {
class Renderder {
 public:
  Renderder();
  void AddMesh(glr::mesh::Mesh* mesh);
  void Update();

 private:
  std::vector<glr::mesh::Mesh*> meshes_;

  GLuint program_ = 0;
  GLint projection_location_ = 0;
  GLint view_location_ = 0;
  GLint model_location_ = 0;
  GLint color_location_ = 0;
  Camera camera_;

  glm::vec4 color_{0.8F, 0.4F, 0.2F, 1.0};
};

}  // namespace glr::render