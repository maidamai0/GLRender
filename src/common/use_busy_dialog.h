#include <atomic>
#include "common/singleton.h"

namespace common {
class use_busy_dialog {
 public:
  use_busy_dialog();
  ~use_busy_dialog();

  no_copy(use_busy_dialog);
  no_move(use_busy_dialog);

  static auto use() -> bool;

 private:
  static std::atomic_char ref_count;
};
}  // namespace common