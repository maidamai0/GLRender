#include "common/log.h"
#include "common/singleton.h"
#include "viewer/main_window.h"

auto main(int argc, char** argv) -> int {
  auto& log = make_singleton<log_details::Log>();
  auto& window = make_singleton<MainWindow>();
  window.Show();
  return 0;
}

#ifdef _WIN32
#include <Windows.h>
auto WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     PWSTR pCmdLine, int nShowCmd) -> int {
  return main(__argc, __argv);
}
#endif