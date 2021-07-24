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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraDirection { kForward, kBackward, kLeft, kRight };

class Camera {
 public:
  explicit Camera(glm::vec3 position = glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F),
                  float yaw = -90.0F, float pitch = 0.0F)
      : position_{position}, world_up_{up}, yaw_{yaw}, pitch_{pitch} {
    update_camera_vectors();
  }

  [[nodiscard]] auto GetViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  [[nodiscard]] auto GetZoom() const {
    return zoom_;
  }

  [[nodiscard]] auto GetPosition() const {
    return position_;
  }

  auto ProcessKeyBoardEvent(CameraDirection direction, float delta_time) {
    auto velocity = move_speed_ * delta_time;
    switch (direction) {
      case CameraDirection::kForward: {
        position_ -= front_ * velocity;
        break;
      }
      case CameraDirection::kBackward: {
        position_ += front_ * velocity;
        break;
      }
      case CameraDirection::kLeft: {
        position_ += right_ * velocity;
        break;
      }
      case CameraDirection::kRight: {
        position_ -= right_ * velocity;
        break;
      }
      default: {
        assert(false && "invalid direction");
        LOGW("warn:invalid direction: {}", direction);
        break;
      }
    }
  }

  auto ProcessMouseMovement(float offset_x, float offset_y, bool constrain_pitch) {
    yaw_ += offset_x * mouse_sensitivity_;
    pitch_ += offset_y * mouse_sensitivity_;

    if (constrain_pitch) {
      if (pitch_ > 89.0F) {
        pitch_ = 89.0F;
      } else if (pitch_ < -89.0F) {
        pitch_ = -89.0F;
      }
    }

    update_camera_vectors();
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
  void update_camera_vectors() {
    glm::vec3 front{static_cast<float>(cos(glm::radians(yaw_)) * cos(glm::radians(pitch_))),
                    static_cast<float>(sin(glm::radians(pitch_))),
                    static_cast<float>(sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)))};
    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }

  // camera attributes
  glm::vec3 position_{0.0F, 0.0F, 0.0F};
  glm::vec3 front_{0.0F, 0.0F, -1.0F};
  glm::vec3 up_{0.0F, 1.0F, 0.0F};
  glm::vec3 right_{1.0F, 0.0F, 0.0F};
  glm::vec3 world_up_{0.0F, 1.0F, 0.0F};

  // camera options
  float move_speed_{250.0F};
  float mouse_sensitivity_{0.1F};
  float zoom_{45.0F};

  // euler angles
  float yaw_{-0.0F};
  float pitch_{0.0F};

  // width/height
  float aspect_ = 1.0F;
};