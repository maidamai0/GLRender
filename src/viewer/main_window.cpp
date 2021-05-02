#include "viewer/main_window.h"

#include <stdexcept>

#include "common/color.h"
#include "common/log.h"
#include "glad/gl.h"
#include "glfw/include/GLFW/glfw3.h"

void glfw_error_callback(int err, const char* msg) {
  LOGE("glfw error:{}[{}]", msg, err);
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action,
                       int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    LOGI("user request to exit");
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

MainWindow::MainWindow() {
  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == 0) {
    LOGE("glfw initialized failed");
    throw std::runtime_error("glfw initialize failed");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  window_ = glfwCreateWindow(800, 600, APP_NAME, nullptr, nullptr);
  if (window_ == nullptr) {
    LOGE("glfw create window failed");
    glfwTerminate();
    throw std::runtime_error("glfw create window failed");
  }

  glfwSetKeyCallback(window_, glfw_key_callback);
  glfwMakeContextCurrent(window_);
  if (gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress)) == 0) {
    LOGE("glad load opengl failed");
    throw std::runtime_error("glad load opengl failed");
  }

  glfwSwapInterval(2);
  glClearColor(COLOR(color::floralwhite), 1.0F);
}

MainWindow::~MainWindow() {
}

void MainWindow::Show() {
  while (glfwWindowShouldClose(window_) == 0) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }
}
