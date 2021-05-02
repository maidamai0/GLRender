#pragma once

struct GLFWwindow;

class MainWindow {
 public:
  MainWindow();
  ~MainWindow();
  void Show();

 private:
  GLFWwindow* window_ = nullptr;
};