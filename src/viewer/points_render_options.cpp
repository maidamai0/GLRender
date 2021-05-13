#include <string>
#include <vector>

#include "fork_awesome.h"
#include "imgui.h"
#include "imgui_helper.hpp"

namespace PointsRenderOptions {
void show() {
  ImGui::Separator();

  {
    static auto index = 0;
    std::vector<const char *> items{"vertices", "licks"};
    ImGuiHelper::Comb(ICON_FK_CUBE, "Drawables", items, index);
    ImGui::Separator();
  }

  {
    static bool visible = true;
    ImGuiHelper::SwitchButton(ICON_FK_EYE, "Visible", visible);
    ImGui::Separator();
  }

  {
    static auto point_width = 1;
    ImGuiHelper::InputInt(ICON_FK_CIRCLE, "Point Width", point_width);
    ImGui::Separator();
  }
}
} // namespace PointsRenderOptions