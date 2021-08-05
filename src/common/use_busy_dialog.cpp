#include "common/use_busy_dialog.h"
#include <atomic>

namespace common {
std::atomic_char use_busy_dialog::ref_count = 0;

use_busy_dialog::use_busy_dialog() {
  ++ref_count;
}

use_busy_dialog::~use_busy_dialog() {
  --ref_count;
}

auto use_busy_dialog::use() -> bool {
  return ref_count > 0;
}
}  // namespace common