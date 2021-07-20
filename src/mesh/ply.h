#pragma once

/**
 * @file ply.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief load and render ply files
 * @version 0.1
 * @date 2021-07-20
 *
 */
#include <string>
#include "mesh/mesh.h"

namespace glr::mesh {
class PLY : public Mesh {
 public:
  explicit PLY(std::string);
  ~PLY() override;
  no_copy(PLY);
  no_move(PLY);
};
}  // namespace glr::mesh