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

namespace io {
enum class Direction { kForward, kBackward, kLeft, kRight };
enum class MouseButton { kLeft, kMiddle, kRight };
enum class MouseAction { kPress, kRelease };
}  // namespace io
