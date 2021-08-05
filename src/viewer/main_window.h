#pragma once

#include <atomic>
#include <memory>
#include "common/singleton.h"
#include "render/renderer.h"

struct GLFWwindow;

class MainWindow {
  enable_singleton(MainWindow);
  MainWindow();

 public:
  ~MainWindow() = default;
  void Show();

 private:
  void on_open_file();
  void spinner();
  void show_spinner(const bool show);

  GLFWwindow* window_ = nullptr;
  std::unique_ptr<glr::render::Renderder> renderer_;
};