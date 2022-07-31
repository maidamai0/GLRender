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

#include "app/app_state.h"
#include "common/log.h"
#include "common/singleton.h"
#include "common/swtich.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "io/input.hpp"
#include "render/trackball.h"

class Camera {
 public:
  Camera() {
    Switch().MousePosition.connect([this](float xpos, float ypos) {
      static bool first_time = true;
      if (first_time) {
        first_time = false;
        return;
      }

      switch (AppState().mouse_mode_) {
        case io::MouseMode::kRotate:
          on_rotate(xpos, ypos);
          break;
        case io::MouseMode::kPan:
          on_pan(xpos, ypos);
          break;
        default:
          break;
      }

      last_mouse_x_ = static_cast<float>(xpos);
      last_mouse_y_ = static_cast<float>(ypos);
    });

    Switch().ResetCamera.connect([this]() {
      position_ = {0.0F, 0.0F, 1.0F};
      AppState().mouse_scroll_y_ = 0.0F;
      trackball_.Reset();
    });
  }

  [[nodiscard]] auto operator()() const {
    auto view = glm::lookAt(position_, front_, up_);
    view = view * trackball_.RotationMatrix();

    const auto zoom = std::clamp(45.0F - AppState().mouse_scroll_y_, 1.0F, 180.0F);
    const auto aspect = static_cast<float>(AppState().window_width_) / static_cast<float>(AppState().window_height_);
    const auto projection = glm::perspective(glm::radians(zoom), aspect, 0.1F, 100.0F);

    return projection * view;
  }

 private:
  void on_rotate(float xpos, float ypos) {
    const auto width = static_cast<float>(AppState().window_width_);
    const auto height = static_cast<float>(AppState().window_height_);

    trackball_.OnMouseMove(last_mouse_x_ / width - 0.5F, -(last_mouse_y_ / height - 0.5F), xpos / width - 0.5F,
                           -(ypos / height - 0.5F));
  }

  void on_pan(float xpos, float ypos) {
    LOGI("pan is broken");
    return;
    const auto xoffset = xpos - last_mouse_x_;
    const auto yoffset = ypos - last_mouse_y_;

    constexpr auto mouse_sensitivity = 0.01F;
    position_ -= right_ * xoffset * mouse_sensitivity;
    position_ -= up_ * yoffset * mouse_sensitivity;
  }

  // camera attributes
  glm::vec3 position_{0.0F, 0.0F, 1.0F};
  glm::vec3 front_{0.0F, 0.0F, 0.0F};
  glm::vec3 up_{0.0F, 1.0F, 0.0F};
  glm::vec3 right_ = glm::normalize(glm::cross(front_ - position_, up_));

  // pan and rorate
  float last_mouse_x_ = 0.0F;
  float last_mouse_y_ = 0.0F;
  TrackBall trackball_;
};