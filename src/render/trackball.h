#pragma once

/**
 * @file trackball.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief implement a simple trackball
 * @version 0.1
 * @date 2022-07-30
 *
 * Copyright (c) 2022 All Rights Reserved
 *
 */

#include <cmath>
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"

class TrackBall {
 public:
  void OnMouseMove(float old_xpos, float old_ypos, float xpos, float ypos) {
    const auto old_pos = glm::vec3(old_xpos, old_ypos, get_z(old_xpos, old_ypos));
    const auto pos = glm::vec3(xpos, ypos, get_z(xpos, ypos));

    const auto axis = glm::normalize(glm::cross(old_pos, pos));
    const auto angle = glm::angle(old_pos, pos) * 10;

    quaternion_ = glm::angleAxis(angle, axis) * quaternion_;
  }

  [[nodiscard("rotation matrix should not be discarded")]] auto RotationMatrix() const {
    return glm::mat4_cast(quaternion_);
  }

  void Reset() {
    quaternion_ = {1, 0, 0, 0};
  }

 private:
  // see https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball for details
  static float get_z(float mouse_x, float mouse_y) {
    const float tmp = std::pow(mouse_x, 2) + std::pow(mouse_y, 2);
    constexpr auto radius = 1.0F;

    // use sphere
    if (tmp < radius / 2) {
      return sqrtf(radius - tmp);
    }

    return radius / 2 / std::sqrtf(tmp);
  }

  glm::quat quaternion_{1, 0, 0, 0};
};
