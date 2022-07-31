#pragma once

#include <string>
#include <utility>
#include "common/singleton.h"
#include "glm/glm.hpp"

namespace glr::mesh {
struct min_max {
  double min = 1.0;
  double max = -1.0;
  [[nodiscard]] auto middle() const -> double {
    return static_cast<float>(min + max) / 2;
  }
};

struct bound {
  min_max x = {};
  min_max y = {};
  min_max z = {};
};

class Mesh {
 public:
  explicit Mesh(std::string file_path) : file_path_(std::move(file_path)) {}
  Mesh() = default;
  virtual ~Mesh() = default;
  virtual void Render() = 0;
  [[nodiscard]] auto Origin() const -> glm::vec3 {
    return {bound_.x.middle(), bound_.y.middle(), bound_.z.middle()};
  }

  no_copy(Mesh);
  no_move(Mesh);

 protected:
  const std::string file_path_;
  bound bound_;
};
}  // namespace glr::mesh