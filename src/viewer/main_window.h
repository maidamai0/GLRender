#pragma once

#include "common/singleton.h"

struct GLFWwindow;

class MainWindow {
  friend auto make_singleton<MainWindow>() -> MainWindow&;
  MainWindow();

 public:
  ~MainWindow() = default;
  void Show();

  MainWindow(const MainWindow&) = delete;
  auto operator=(const MainWindow&) -> MainWindow& = delete;

  MainWindow(MainWindow&&) = delete;
  auto operator=(MainWindow&&) -> MainWindow& = delete;

 private:
  GLFWwindow* window_ = nullptr;
};