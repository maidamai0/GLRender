#pragma once
/**
 * @file camera.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief Camera
 * @version 0.1
 * @date 2021-07-24
 */

#include <cassert>

#include "common/log.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

enum class CameraDirection { kForward, kBackward, kLeft, kRight };

class Camera {
 public:
  Camera() = default;

  [[nodiscard]] auto GetViewMatrix() const {
    return view_;
  }

  [[nodiscard]] auto GetZoom() const {
    return zoom_;
  }

  auto ProcessMouseMovement(float offset_x, float offset_y, bool constrain_pitch) {
    view_ = glm::rotate(view_, glm::radians(offset_x), glm::vec3(0.0F, 1.0F, 0.0F));
    view_ = glm::rotate(view_, glm::radians(offset_y), glm::vec3(1.0F, 0.0F, 0.0F));
  }

  auto ProcessMouseScroll(float yoffset) {
    zoom_ -= yoffset;
  }

  auto SetAspect(const float aspect) {
    aspect_ = aspect;
  }

  [[nodiscard]] auto GetAspect() const {
    return aspect_;
  }

 private:
  glm::mat4 view_ = glm::lookAt(glm::vec3(0.0, 0.0, 1.0),   // eye
                                glm::vec3(0.0, 0.0, 0.0),   // direction
                                glm::vec3(0.0, 1.0, 0.0));  // up
  float zoom_{45.0F};

  // width/height
  float aspect_ = 1.0F;
};