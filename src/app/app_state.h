#pragma once

/**
 * @file app_state.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-07-31
 *
 * Copyright (c) 2022 All Rights Reserved
 *
 */

#include <string>
#include <unordered_map>
#include <variant>
#include "common/singleton.h"
#include "glm/vec4.hpp"
#include "io/input.hpp"

namespace app {
class State {
 private:
  enable_singleton(State);
  State() = default;

 public:
  ~State() = default;

  // window
  int window_width_ = 1270;
  int window_height_ = 768;

  // mouse
  float mouse_xpos_ = 0.0F;
  float mouse_ypos_ = 0.0F;
  float mouse_scroll_x_ = 0.0F;
  float mouse_scroll_y_ = 0.0F;
  io::MouseMode mouse_mode_ = io::MouseMode::kNone;

  glm::vec4 mesh_color_{0.8F, 0.4F, 0.2F, 1.0};
};

}  // namespace app

inline auto AppState() -> app::State& {
  return app::State::Instance();
}