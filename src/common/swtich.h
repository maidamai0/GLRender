#pragma once

#include "common/singleton.h"
#include "glm/fwd.hpp"
#include "signal_slot/nano_signal_slot.hpp"

namespace common {
class Switch {
  enable_singleton(Switch);

 public:
  Switch() = default;
  ~Switch() = default;
  Nano::Signal<void(const float zoom)> ZoomChanged;
  Nano::Signal<void(const float zoom)> AspectChanged;
  Nano::Signal<void(const glm::vec3& color)> ColorChanged;
};
}  // namespace common