#include <string>
#include <vector>

#include "fork_awesome.h"
#include "imgui.h"
#include "imgui_helper.hpp"

namespace LineRenderOptions {
void show() {
  ImGui::Separator();

  {
    static auto index = 0;
    std::vector<const char *> items{"edges", "borders"};
    ImGuiHelper::Comb(std::string(ICON_FK_CUBE), "Drawables", items, index);
    ImGui::Separator();
  }

  {
    static bool visible = true;
    ImGuiHelper::SwitchButton(ICON_FK_EYE, "Visible", visible);
    ImGui::Separator();
  }

  {
    static auto line_width = 1;
    ImGuiHelper::InputInt(ICON_FK_BARCODE, "Line Width", line_width);
    ImGui::Separator();
  }

  {
    static auto imposter_index = 0;
    std::vector<const char *> items{"plain", "cylider", "cone"};
    ImGuiHelper::Comb(ICON_FK_HASHTAG, "Imposter", items, imposter_index);
  }
}
} // namespace LineRenderOptions