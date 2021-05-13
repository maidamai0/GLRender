#include "viewer/main_window.h"

#include <stdexcept>

#include "common/color.h"
#include "common/log.h"
#include "glad/gl.h"
#include "glfw/include/GLFW/glfw3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "stb/stb_image.h"
#include "viewer/icon.png.h"
#include "viewer/render_options_panel.h"
#include "viewer/style.h"

namespace {
constexpr auto kGLSLVersion = "#version 130";
}

void glfw_error_callback(int err, const char* msg) {
  LOGE("glfw error:{}[{}]", msg, err);
}

void glfw_resize_callback(GLFWwindow* wind, int width, int height) {
  glfwSwapBuffers(wind);
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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window_ = glfwCreateWindow(800, 600, APP_NAME, nullptr, nullptr);
  if (window_ == nullptr) {
    LOGE("glfw create window failed");
    glfwTerminate();
    throw std::runtime_error("glfw create window failed");
  }

  glfwSetKeyCallback(window_, glfw_key_callback);
  glfwSetWindowSizeCallback(window_, glfw_resize_callback);
  glfwMakeContextCurrent(window_);
  if (gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress)) == 0) {
    LOGE("glad load opengl failed");
    throw std::runtime_error("glad load opengl failed");
  }

  //  set window icon
  {
    GLFWimage icon;
    icon.pixels = stbi_load_from_memory(icon_png, sizeof(icon_png), &icon.width,
                                        &icon.height, nullptr, 4);
    glfwSetWindowIcon(window_, 1, &icon);
    stbi_image_free(icon.pixels);
  }

  glfwSwapInterval(2);
  glClearColor(COLOR(color::floralwhite), 1.0F);

  // imgui initialize
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init(kGLSLVersion);

  style::init_style(1.0F, 1.0F);
}

void MainWindow::Show() {
  auto show_demo_window = true;
  while (glfwWindowShouldClose(window_) == 0) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderOptionsPanel::show();

    ImGui::Render();
    int display_w = 0;
    int display_h = 0;
    glfwGetFramebufferSize(window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window_);
  glfwTerminate();
}
