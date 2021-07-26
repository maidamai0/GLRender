#include <string>
#include <vector>

#include "fork_awesome.h"
#include "imgui.h"
#include "imgui_helper.hpp"

namespace PointsRenderOptions {
void show() {
  ImGuiHelper::ListSeparator();

  {
    static auto index = 0;
    std::vector<const char *> items{"vertices", "licks"};
    ImGuiHelper::Comb(ICON_FK_CUBE, "Drawables", items, index);
    ImGuiHelper::ListSeparator();
  }

  {
    static bool visible = true;
    ImGuiHelper::SwitchButton(ICON_FK_EYE, "Visible", visible);
    ImGuiHelper::ListSeparator();
  }

  {
    static auto point_width = 1;
    ImGuiHelper::InputInt(ICON_FK_CIRCLE, "Point Width", point_width);
  }
}
} // namespace PointsRenderOptions