#include "viewer/main_window.h"
#include "common/color.h"
#include "common/log.h"
#include "common/singleton.h"
#include "common/swtich.h"
#include "mesh/ply.h"
#include "mesh/triangle.h"
#include "render/renderer.h"
#include "viewer/icon.png.h"
#include "viewer/render_options_panel.h"
#include "viewer/style.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "stb/stb_image.h"

#include <string>

namespace {
constexpr auto kGLSLVersion = "#version 410";
}  // namespace

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
  window_ = glfwCreateWindow(800, 600, APP_NAME, nullptr, nullptr);
  if (window_ == nullptr) {
    LOGE("glfw create window failed");
    glfwTerminate();
    throw std::runtime_error("glfw create window failed");
  }

  glfwSetKeyCallback(window_, glfw_key_callback);
  glfwSetWindowSizeCallback(window_, glfw_resize_callback);
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
  glr::mesh::Triangle triangle(window_);
  glr::mesh::PLY bunny("models/bunny.ply");

  // FIXME (tonghao): 2021-07-24
  // use shared_ptr
  glr::render::Renderder renderer;
  // renderer.AddMesh(&triangle);
  renderer.AddMesh(&bunny);

  while (glfwWindowShouldClose(window_) == 0) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    int display_w = 0;
    int display_h = 0;
    glfwGetFramebufferSize(window_, &display_w, &display_h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    make_singleton<common::Switch>().ZoomChanged.fire(ImGui::GetIO().MouseWheel);
    make_singleton<common::Switch>().AspectChanged.fire(
        (static_cast<float>(display_w) / static_cast<float>(display_h)));
    if (ImGui::IsMouseDown(0)) {
      make_singleton<common::Switch>().YawPitchChanged.fire(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);
    }

    RenderOptionsPanel::show();
    renderer.Update();

    ImGui::Render();
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
