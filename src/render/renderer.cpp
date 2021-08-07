#include "render/renderer.h"
#include "common/singleton.h"
#include "common/swtich.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// vertex shader
static constexpr auto vertex_shader_text = R"(
#version 410

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0);
};
)";

// fragment shader
static constexpr auto fragment_shader_text = R"(
#version 410

uniform vec3 color;

void main() {
  gl_FragColor = vec4(color, 1.0);
};
)";

namespace glr::render {
Renderder::Renderder() {
  // vertest shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
  glCompileShader(vertex_shader);

  // fragment shader
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
  glCompileShader(fragment_shader);

  program_ = glCreateProgram();
  glAttachShader(program_, vertex_shader);
  glAttachShader(program_, fragment_shader);
  glLinkProgram(program_);

  projection_location_ = glGetUniformLocation(program_, "projection");
  view_location_ = glGetUniformLocation(program_, "view");
  model_location_ = glGetUniformLocation(program_, "model");
  color_location_ = glGetUniformLocation(program_, "color");

  // set draw type
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  make_singleton<common::Switch>().ZoomChanged.connect<&Renderder::on_zoom_chaned>(this);
  make_singleton<common::Switch>().AspectChanged.connect<&Renderder::on_aspect_changed>(this);
  make_singleton<common::Switch>().YawPitchChanged.connect<&Renderder::on_yaw_pich_changed>(this);
}

void Renderder::AddMesh(glr::mesh::Mesh* mesh) {
  meshes_.push_back(mesh);
}

void Renderder::Update() {
  glUseProgram(program_);

  glm::mat4 projection = glm::perspective(glm::radians(camera_.GetZoom()), camera_.GetAspect(), 0.1F, 100.0F);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 model = glm::mat4(1.0F);

  glUniformMatrix4fv(projection_location_, 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(view_location_, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(model_location_, 1, GL_FALSE, glm::value_ptr(model));
  glUniform3fv(color_location_, 1, glm::value_ptr(color_));

  for (auto* mesh : meshes_) {
    mesh->Render();
  }
}

void Renderder::on_zoom_chaned(const float zoom) {
  camera_.ProcessMouseScroll(zoom);
}

void Renderder::on_aspect_changed(const float aspect) {
  camera_.SetAspect(aspect);
}

void Renderder::on_yaw_pich_changed(const int yaw, const int pitch) {
  camera_.ProcessMouseMovement(static_cast<float>(-yaw), static_cast<float>(pitch), true);
}

void Renderder::on_color_changed(const glm::vec3& color) {
  color_ = color;
}
}  // namespace glr::render