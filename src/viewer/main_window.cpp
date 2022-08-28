#include "viewer/main_window.h"
#include "app/app_state.h"
#include "common/color.h"
#include "common/log.h"
#include "common/singleton.h"
#include "common/swtich.h"
#include "common/use_busy_dialog.h"
#include "io/input.hpp"
#include "mesh/cube.h"
#include "mesh/ply.h"
#include "mesh/triangle.h"
#include "render/camera.h"
#include "render/renderer.h"
#include "viewer/icon.png.h"
#include "viewer/render_options_panel.h"
#include "viewer/style.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "nfd.hpp"
#include "stb/stb_image.h"

#include <array>
#include <cmath>
#include <filesystem>
#include <memory>
#include <string>

namespace {
constexpr auto kGLSLVersion = "#version 410";
}  // namespace

void MainWindow::spinner() {
  int window_width = 0;
  int window_height = 0;
  const auto flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
  glfwGetWindowSize(window_, &window_width, &window_height);
  ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)));
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowBgAlpha(0.5F);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetColorU32(ImGuiCol_TitleBgActive));

  ImGui::Begin("##SinnerWindow", nullptr, flags);
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems) {
    return;
  }

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID("##spinner");

  ImVec2 size{60, 60};
  ImVec2 pos = {(ImGui::GetWindowWidth() - size.x) / 2, (ImGui::GetWindowHeight() - size.y) / 2};
  const auto radius = size.x / 2;

  const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
  ImGui::ItemSize(bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(bb, id)) {
    return;
  }

  window->DrawList->PathClear();

  int num_segments = 300;
  int start = abs(ImSin(g.Time * 1.8F) * (num_segments - 5));
  const float a_min = IM_PI * 2.0F * (static_cast<float>(start) / static_cast<float>(num_segments));
  const float a_max = IM_PI * 2.0F * (static_cast<float>(num_segments - 200) / static_cast<float>(num_segments));

  const ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);

  for (int i = 0; i < num_segments; ++i) {
    const float a = a_min + (a_max - a_min) * (static_cast<float>(i) / static_cast<float>(num_segments));
    window->DrawList->PathLineTo(
        ImVec2(center.x + ImCos(a + g.Time * 8) * radius, center.y + ImSin(a + g.Time * 8) * radius));
  }

  ImGui::PopStyleColor();
  window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_ButtonActive), 0, 3.0F);
  ImGui::PopStyleVar(3);
  ImGui::End();
}

void glfw_error_callback(int err, const char* msg) {
  LOGE("glfw error:{}[{}]", msg, err);
}

void glfw_resize_callback(GLFWwindow* wind, int width, int height) {
  // window is minimized
  if (width <= 0 || height <= 0) {
    return;
  }

  glfwSwapBuffers(wind);
  AppState().window_width_ = width;
  AppState().window_height_ = height;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action,
                       int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    LOGI("user request to exit");
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void glfw_mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
  if (ImGui::GetIO().WantCaptureMouse) {
    return;
  }

  AppState().mouse_xpos_ = static_cast<float>(xpos);
  AppState().mouse_ypos_ = static_cast<float>(ypos);
  Switch().MousePosition(static_cast<float>(xpos), static_cast<float>(ypos));
}

void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
  if (ImGui::GetIO().WantCaptureMouse) {
    return;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    AppState().mouse_mode_ = action == GLFW_PRESS ? io::MouseMode::kRotate : io::MouseMode::kNone;
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    AppState().mouse_mode_ = action == GLFW_PRESS ? io::MouseMode::kPan : io::MouseMode::kNone;
  }

  if (AppState().mouse_mode_ != io::MouseMode::kNone) {
    LOGI("mouse mode: {}", io::MouseModeStr(AppState().mouse_mode_));
  }
}

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
  if (ImGui::GetIO().WantCaptureMouse) {
    return;
  }

  AppState().mouse_scroll_x_ += static_cast<float>(xoffset);
  AppState().mouse_scroll_y_ += static_cast<float>(yoffset);
}

void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length, const GLchar* msg,
                            const void* data) {
  std::string _source;
  std::string _type;
  std::string _severity;

  switch (source) {
    case GL_DEBUG_SOURCE_API:
      _source = "API";
      break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      _source = "WINDOW SYSTEM";
      break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      _source = "SHADER COMPILER";
      break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
      _source = "THIRD PARTY";
      break;

    case GL_DEBUG_SOURCE_APPLICATION:
      _source = "APPLICATION";
      break;

    case GL_DEBUG_SOURCE_OTHER:
      _source = "UNKNOWN";
      break;

    default:
      _source = "UNKNOWN";
      break;
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      _type = "ERROR";
      break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      _type = "DEPRECATED BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      _type = "UDEFINED BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_PORTABILITY:
      _type = "PORTABILITY";
      break;

    case GL_DEBUG_TYPE_PERFORMANCE:
      _type = "PERFORMANCE";
      break;

    case GL_DEBUG_TYPE_OTHER:
      _type = "OTHER";
      break;

    case GL_DEBUG_TYPE_MARKER:
      _type = "MARKER";
      break;

    default:
      _type = "UNKNOWN";
      break;
  }

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      _severity = "HIGH";
      break;

    case GL_DEBUG_SEVERITY_MEDIUM:
      _severity = "MEDIUM";
      break;

    case GL_DEBUG_SEVERITY_LOW:
      _severity = "LOW";
      break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
      _severity = "NOTIFICATION";
      break;

    default:
      _severity = "UNKNOWN";
      break;
  }

  LOGD("{}: {} of {} severity, raised from {}: {}", id, _type, _severity,
       _source, msg);
}

MainWindow::MainWindow() {
  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == 0) {
    LOGE("glfw initialized failed");
    throw std::runtime_error("glfw initialize failed");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window_ = glfwCreateWindow(AppState().window_width_, AppState().window_height_, APP_NAME, nullptr, nullptr);
  if (window_ == nullptr) {
    LOGE("glfw create window failed");
    glfwTerminate();
    throw std::runtime_error("glfw create window failed");
  }

  glfwSetKeyCallback(window_, glfw_key_callback);
  glfwSetWindowSizeCallback(window_, glfw_resize_callback);
  glfwSetMouseButtonCallback(window_, glfw_mouse_callback);
  glfwSetCursorPosCallback(window_, glfw_mouse_move_callback);
  glfwSetScrollCallback(window_, glfw_scroll_callback);
  glfwMakeContextCurrent(window_);
  if (gladLoadGL() == 0) {
    LOGE("glad load opengl failed");
    throw std::runtime_error("glad load opengl failed");
  }

  // see https://gist.github.com/liam-middlebrook/c52b069e4be2d87a6d2f for
  // details
  // =============== INIT DEBUG OUTPUT ================
  // The following function calls should be made directly after OpenGL
  // initialization.

  // Enable the debugging layer of OpenGL
  //
  // GL_DEBUG_OUTPUT - Faster version but not useful for breakpoints
  // GL_DEBUG_OUTPUT_SYNCHRONOUS - Callback is in sync with errors, so a
  // breakpoint can be placed on the callback in order to get a stacktrace for
  // the GL error.

  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  // Set the function that will be triggered by the callback, the second
  // parameter is the data parameter of the callback, it can be useful for
  // different contexts but isn't necessary for our simple use case.
  glDebugMessageCallback(GLDebugMessageCallback, nullptr);
  glEnable(GL_DEPTH_TEST);

  //  set window icon
  {
    GLFWimage icon;
    icon.pixels = stbi_load_from_memory(icon_png, sizeof(icon_png), &icon.width,
                                        &icon.height, nullptr, 4);
    glfwSetWindowIcon(window_, 1, &icon);
    stbi_image_free(icon.pixels);
  }

  glfwSwapInterval(2);
  glClearColor(COLOR(color::white), 1.0F);

  // imgui initialize
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window_, false);
  ImGui_ImplOpenGL3_Init(kGLSLVersion);

  renderer_ = std::make_unique<glr::render::Renderder>();
  NFD::Init();
  Switch().OpenFile.connect(&MainWindow::on_open_file, this);
  style::init_style(1.0F, 1.0F);
}

void MainWindow::Show() {
  while (glfwWindowShouldClose(window_) == 0) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    int display_w = 0;
    int display_h = 0;
    glfwGetFramebufferSize(window_, &display_w, &display_h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderOptionsPanel::show();
    renderer_->Update();
    if (common::use_busy_dialog::use()) {
      spinner();
    }

    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  NFD::Quit();
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void MainWindow::on_open_file() {
  nfdchar_t* out_path = nullptr;
  std::array<nfdfilteritem_t, 1> filterItem = {{"mesh", "ply"}};
  nfdresult_t result = NFD_OpenDialog(&out_path, filterItem.data(), filterItem.size(), nullptr);
  if (result == NFD_OKAY) {
    const auto file_path = std::filesystem::path(out_path).generic_string();
    renderer_->AddMesh(std::make_unique<glr::mesh::PLY>(file_path));
    glfwSetWindowTitle(window_, file_path.c_str());
    LOGI(file_path);
    NFD_FreePath(out_path);
  } else if (result == NFD_CANCEL) {
    LOGW("User pressed cancel");
  } else {
    LOGW("Open file faield:{}", NFD_GetError());
  }
}