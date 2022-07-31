#include "render/renderer.h"
#include "app/app_state.h"
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

uniform mat4 mvp;

void main() {
  gl_Position = mvp * vec4(position, 1.0);
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

  mvp_location_ = glGetUniformLocation(program_, "mvp");
  color_location_ = glGetUniformLocation(program_, "color");
}

void Renderder::AddMesh(glr::mesh::Mesh* mesh) {
  meshes_.push_back(mesh);
}

void Renderder::Update() {
  glUseProgram(program_);

  const auto proj_view = camera_();
  glUniform4fv(color_location_, 1, glm::value_ptr(AppState().mesh_color_));

  // set draw type
  int draw_mode = GL_FILL;
  if (AppState().draw_mode_ == app::DrawMode::Lines) {
    draw_mode = GL_LINE;
  } else if (AppState().draw_mode_ == app::DrawMode::Points) {
    draw_mode = GL_POINT;
  }
  glPolygonMode(GL_FRONT_AND_BACK, draw_mode);

  for (auto* mesh : meshes_) {
    glm::mat4 model = glm::mat4(1.0F);
    model = glm::translate(model, -mesh->Origin());
    glUniformMatrix4fv(mvp_location_, 1, GL_FALSE, glm::value_ptr(proj_view * model));
    mesh->Render();
  }
}

}  // namespace glr::render