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
#include "glm/vec3.hpp"
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

  // renderering
  glm::vec4 mesh_color_{0.761F, 0.761F, 0.761F, 1.0};
  glm::vec4 line_color_{0.0F, 0.0F, 0.0F, 1.0};
  glm::vec4 point_color_{0.0F, 0.0F, 0.0F, 1.0};
  glm::vec3 light_pos_{0.0F, 1.0F, 0.0F};
  bool draw_line_{false};
  bool draw_points{false};
  bool draw_face{true};
  bool enable_cull_face{false};
  int cull_face{1};   // 0 for font, 1 for back, 2 for front and back
  int front_face{1};  // 0 for cw, 1 for ccw
};

}  // namespace app

inline auto AppState() -> app::State& {
  return app::State::Instance();
}