#include "render/renderer.h"
#include <algorithm>
#include <stdexcept>
#include "app/app_state.h"
#include "common/log.h"
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
layout(location = 1) in vec3 normal;

uniform mat4 mvp;
uniform vec3 light_pos = vec3(0, 1, 0);
uniform vec3 camera_pos = vec3(0, 0, 1);

out float s;

void main() {
  gl_Position = mvp * vec4(position, 1.0);

  vec3 light_dir = normalize(gl_Position.xyz - light_pos);
  vec3 reflect_dir = normalize(2 * dot(normal, light_dir) * normal - light_dir);

  vec3 view_dir = normalize(gl_Position.xyz - camera_pos);

  s = dot(view_dir, reflect_dir);
};
)";

// fragment shader
static constexpr auto fragment_shader_text = R"(
#version 410

uniform vec4 color;
in float s;

void main() {
  gl_FragColor = vec4(vec3(color.xyz) * s, 1.0);
};
)";

static auto check_compile(const GLuint shader, const GLchar* const shader_source) -> bool {
  GLint success = 0;
  std::array<GLchar, 512> msg = {0};

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE) {
    glGetShaderInfoLog(shader, msg.size(), nullptr, msg.data());
    LOGE("compile shader failed:{}\nsource:{}\n", msg.data(), shader_source);
    throw std::runtime_error("shader compile error");
  }

  return true;
}

static auto check_link(const GLuint link) -> bool {
  GLint success = 0;
  std::array<GLchar, 512> msg = {0};

  glGetProgramiv(link, GL_LINK_STATUS, &success);
  if (success != GL_TRUE) {
    glGetShaderInfoLog(link, msg.size(), nullptr, msg.data());
    LOGE("compile shader failed:{}\n", msg.data());
    throw std::runtime_error("shader program link error");
  }

  return true;
}

namespace glr::render {
Renderder::Renderder() {
  // vertest shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
  glCompileShader(vertex_shader);
  if (!check_compile(vertex_shader, vertex_shader_text)) {
    throw std::runtime_error("invalid vertex shader");
  }

  // fragment shader
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
  glCompileShader(fragment_shader);
  if (!check_compile(fragment_shader, fragment_shader_text)) {
    throw std::runtime_error("invalid fragment shader");
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vertex_shader);
  glAttachShader(program_, fragment_shader);
  glLinkProgram(program_);
  if (!check_link(program_)) {
    throw std::runtime_error("invalid shader program");
  }

  mvp_location_ = glGetUniformLocation(program_, "mvp");
  color_location_ = glGetUniformLocation(program_, "color");
  light_location_ = glGetUniformLocation(program_, "light_pos");
  camera_location_ = glGetUniformLocation(program_, "camera_pos");
}

void Renderder::AddMesh(std::unique_ptr<glr::mesh::Mesh> mesh) {
  meshes_.emplace_back(std::move(mesh));
}

void Renderder::Update() {
  glUseProgram(program_);

  const auto proj_view = camera_();

  // set draw type
  for (auto& mesh : meshes_) {
    glm::mat4 model = glm::mat4(1.0F);
    model = glm::translate(model, -mesh->Center());
    glUniformMatrix4fv(mvp_location_, 1, GL_FALSE, glm::value_ptr(proj_view * model));
    glUniform3f(light_location_, AppState().light_pos_.x, AppState().light_pos_.y, AppState().light_pos_.z);

    // draw face
    if (AppState().draw_face) {
      glUniform4fv(color_location_, 1, glm::value_ptr(AppState().mesh_color_));
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      mesh->Render();
    }

    // draw line
    if (AppState().draw_line_) {
      glUniform4fv(color_location_, 1, glm::value_ptr(AppState().line_color_));
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      mesh->Render();
    }

    // draw points
    if (AppState().draw_points) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      glPointSize(3.0);
      glUniform4fv(color_location_, 1, glm::value_ptr(AppState().point_color_));
      mesh->Render();
    }
  }
}

}  // namespace glr::render