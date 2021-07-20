#pragma once

#include "common/singleton.h"

struct GLFWwindow;

class MainWindow {
  enable_singleton(MainWindow);
  MainWindow();

 public:
  ~MainWindow() = default;
  void Show();

 private:
  GLFWwindow* window_ = nullptr;
};