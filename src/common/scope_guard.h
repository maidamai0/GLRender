#pragma once

#include "common/singleton.h"

namespace common {
template <typename Func>
class scope_guard {
 public:
  explicit scope_guard(Func f) : func_(f) {
  }

  ~scope_guard() {
    func_();
  }

 private:
  Func func_;
};

template <typename Func>
auto make_scope_guard(Func f) -> scope_guard<Func> {
  return scope_guard<Func>(f);
}

}  // namespace common