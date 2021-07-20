
#include "mesh/ply.h"
#include <utility>

namespace glr::mesh {
PLY::PLY(std::string path) : Mesh(std::move(path)) {
}
}  // namespace glr::mesh