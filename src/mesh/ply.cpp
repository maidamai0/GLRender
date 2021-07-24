#include "mesh/ply.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "common/log.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "happly/happly.h"
#include "spdlog/fmt/bundled/format.h"

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

namespace glr::mesh {
PLY::PLY(GLFWwindow* window, std::string path) : Mesh(std::move(path)), window_(window) {
  happly::PLYData file(file_path_);
  vertices_ = file.getVertexPositions();
  for (const auto f : file.getFaceIndices<GLuint>()) {
    for (const auto& i : f) {
      faces_indices_.push_back(i);
    }
  }

  LOGI("number of vertices is {}", vertices_.size());
  LOGI("size of of vertice data is {}", sizeof(vertices_[0]) * vertices_.size());
  LOGI("number of faces is {}", faces_indices_.size() / 3);
  LOGI("size of of faces data is {}", faces_indices_.size());

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

  // vao
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // vertex buffer
  GLuint vertex_buffer{0};
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_[0]) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), nullptr);
  glEnableVertexAttribArray(0);

  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * faces_indices_.size(), faces_indices_.data(), GL_STATIC_DRAW);

  // set draw type
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void PLY::Render() {
  int height = 0;
  int width = 0;
  glfwGetFramebufferSize(window_, &width, &height);
  float ratio = width / static_cast<float>(height);

  glm::mat4 projection = glm::perspective(glm::radians(20.0F), ratio, 0.1F, 100.0F);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 model = glm::scale(glm::mat4(1.0F), glm::vec3(5.0F, 5.0F, 5.0F));
  model = glm::translate(model, glm::vec3(0.0F, -0.1F, -0.0F));

  glUseProgram(program_);
  glBindVertexArray(vao_);
  glUniformMatrix4fv(projection_location_, 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(view_location_, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(model_location_, 1, GL_FALSE, glm::value_ptr(model));
  glUniform3fv(color_location_, 1, glm::value_ptr(glm::vec3(0.8F, 0.4F, 0.2F)));
  glDrawElements(GL_TRIANGLES, faces_indices_.size(), GL_UNSIGNED_INT, nullptr);
}
}  // namespace glr::mesh