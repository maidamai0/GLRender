#pragma once

#include <string>
#include <utility>
#include "common/singleton.h"

namespace glr::mesh {
class Mesh {
 public:
  explicit Mesh(std::string file_path) : file_path_(std::move(file_path)) {}
  Mesh() = default;
  virtual ~Mesh() = default;
  virtual void Render() = 0;

  no_copy(Mesh);
  no_move(Mesh);

 private:
  const std::string file_path_;
};
}  // namespace glr::mesh