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
  GLint mvp_location_ = 0;
  GLint color_location_ = 0;
  Camera camera_;
};

}  // namespace glr::render