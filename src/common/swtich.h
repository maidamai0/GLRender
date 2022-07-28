#pragma once

#include "common/singleton.h"
#include "glm/fwd.hpp"
#include "io/input.hpp"
#include "sigslot/signal.hpp"

namespace common {
class Switch {
  enable_singleton(Switch);

 public:
  Switch() = default;
  ~Switch() = default;

  sigslot::signal<float> Zoom;
  sigslot::signal<float> Aspect;
  sigslot::signal_st<const glm::vec4&> MeshColor;
  sigslot::signal<double, double> MousePosition;
  sigslot::signal<io::MouseButton, io::MouseAction> MouseButtonAction;
  sigslot::signal<> OpenFile;
};
}  // namespace common