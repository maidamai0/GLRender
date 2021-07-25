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
  void on_zoom_chaned(const float zoom);
  void on_aspect_changed(const float aspect);
  void on_yaw_pich_changed(const int yaw, const int pitch);
  void on_color_changed(const glm::vec3& color);

  std::vector<glr::mesh::Mesh*> meshes_;

  GLuint program_ = 0;
  GLint projection_location_ = 0;
  GLint view_location_ = 0;
  GLint model_location_ = 0;
  GLint color_location_ = 0;
  Camera camera_{{0.0F, 0.0F, 5.0F}};

  glm::vec3 color_{0.8F, 0.4F, 0.2F};
};

}  // namespace glr::render