/**
 * @file input.hpp
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-07-28
 *
 * Copyright (c) 2022 All Rights Reserved
 *
 */

#pragma once

#include <string>

namespace io {
enum class Direction { kForward, kBackward, kLeft, kRight };
enum class MouseButton { kLeft, kMiddle, kRight };
enum class MouseAction { kPress, kRelease };
enum class MouseMode { kRotate, kPan, kNone };

inline auto MouseModeStr(MouseMode mode) -> std::string {
  switch (mode) {
    case MouseMode::kNone:
      return "None";
    case MouseMode::kRotate:
      return "Rotate";
    case MouseMode::kPan:
      return "Pan";
  }
}
}  // namespace io
