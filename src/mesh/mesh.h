#pragma once

#include <filesystem>
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
  explicit Mesh(const std::filesystem::path& file_path = {}) : file_path_(file_path.generic_string()) {
  }
  virtual ~Mesh() = default;

  virtual void Render() = 0;

  [[nodiscard]] auto Center() const -> glm::vec3 {
    return {bound_.x.middle(), bound_.y.middle(), bound_.z.middle()};
  }

  no_copy(Mesh);
  no_move(Mesh);

 protected:
  const std::string file_path_;
  bound bound_;
};
}  // namespace glr::mesh