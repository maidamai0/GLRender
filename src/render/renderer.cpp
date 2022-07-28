#include "render/renderer.h"
#include "common/singleton.h"
#include "common/swtich.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"

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

uniform vec4 color;

void main() {
  gl_FragColor = color;
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

  make_singleton<common::Switch>().MeshColor.connect(
      [this](const glm::vec4& mesh_color) { this->color_ = mesh_color; });
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
  glUniform4fv(color_location_, 1, glm::value_ptr(color_));

  for (auto* mesh : meshes_) {
    mesh->Render();
  }
}

}  // namespace glr::render