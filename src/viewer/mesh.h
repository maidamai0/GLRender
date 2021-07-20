#pragma once

#include "glad/gl.h"
#include "glfw/deps/linmath.h"
#include "glfw/include/GLFW/glfw3.h"

class Mesh {
 private:
  // vertex
  static constexpr struct {
    float x, y;
    float r, g, b;
  } vertices_data[3] = {{-0.6F, -0.4F, 1.F, 0.F, 0.F},
                        {0.6F, -0.4F, 0.F, 1.F, 0.F},
                        {0.F, 0.6F, 0.F, 0.F, 1.F}};

  // vertex shader
  static constexpr auto vertex_shader_text =
      "#version 410\n"
      "uniform mat4 MVP;\n"
      "in vec3 Color;\n"
      "in vec2 Position;\n"
      "out vec3 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    gl_Position = MVP * vec4(Position, 0.0, 1.0);\n"
      "    Frag_Color = Color;\n"
      "}\n";

  // fragment shader
  static constexpr auto fragment_shader_text =
      "#version 410\n"
      "in vec3 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    gl_FragColor = vec4(Frag_Color, 1.0);\n"
      "}\n";

 public:
  explicit Mesh(GLFWwindow* window) : window_(window) {
    // vertex buffer
    GLuint vertex_buffer{0};
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data,
                 GL_STATIC_DRAW);

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

    mvp_location_ = glGetUniformLocation(program_, "MVP");
    GLuint vpos_location = glGetAttribLocation(program_, "Position");
    GLuint vcol_location = glGetAttribLocation(program_, "Color");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices_data[0]), (void*)(0));

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices_data[0]), (void*)(sizeof(float) * 2));
  }

  void Render() {
    mat4x4 m;
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, static_cast<float>(glfwGetTime()));

    int height = 0, width = 0;
    glfwGetFramebufferSize(window_, &width, &height);
    float ratio = width / static_cast<float>(height);

    mat4x4 p;
    mat4x4_ortho(p, -ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

    mat4x4 mvp;
    mat4x4_mul(mvp, p, m);

    glUseProgram(program_);
    glUniformMatrix4fv(mvp_location_, 1, GL_FALSE, (const GLfloat*)mvp);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

 private:
  GLFWwindow* window_ = nullptr;
  GLuint program_ = 0;
  GLuint mvp_location_ = 0;
};