#pragma once
/**
 * @file camera.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief Camera
 * @version 0.1
 * @date 2021-07-24
 */

#include <algorithm>
#include <cassert>

#include "common/log.h"
#include "common/singleton.h"
#include "common/swtich.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "io/input.hpp"

class Camera {
 public:
  Camera() {
    make_singleton<common::Switch>().Zoom.connect([this](float zoom) { this->zoom_ = zoom; });
    make_singleton<common::Switch>().Aspect.connect([this](float aspect) { this->aspect_ = aspect; });
    make_singleton<common::Switch>().MouseButtonAction.connect(
        [this](io::MouseButton button, io::MouseAction action) { LOGW("MouseButtonAction event not implemented!"); });
    make_singleton<common::Switch>().MousePosition.connect(
        [this](double xpos, double ypos) { LOGW("MousePosition event not implemented"); });
  }

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
    zoom_ = std::clamp(zoom_, 10.0F, 180.0F);
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