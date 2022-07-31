#pragma once

#include "common/singleton.h"
#include "glm/fwd.hpp"
#include "io/input.hpp"
#include "sigslot/signal.hpp"

namespace common {
class Switch {
  enable_singleton(Switch);
  Switch() = default;

 public:
  ~Switch() = default;

  sigslot::signal<float, float> MousePosition;
  sigslot::signal<> OpenFile;
  sigslot::signal<> ResetCamera;
};
}  // namespace common

inline auto Switch() -> common::Switch& {
  return common::Switch::Instance();
}