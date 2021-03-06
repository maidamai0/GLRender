#include <array>
#include <string>
#include <vector>
#include "fork_awesome.h"
#include "imgui.h"
#include "imgui_helper.hpp"
#include "viewer/layout.h"

namespace SurfaceRenderOptions {
void show() {
  ImGuiHelper::ListSeparator();

  {

    static auto index = 0;
    std::vector<const char *> items{"faces", "lines"};
    ImGuiHelper::Comb(ICON_FK_CUBE, "Drawables", items, index);
    ImGuiHelper::ListSeparator();
  }

  {
    static bool visible = true;
    ImGuiHelper::SwitchButton(ICON_FK_EYE, "Visible", visible);
    ImGuiHelper::ListSeparator();
  }

  {

    static bool phong_shading = true;
    ImGuiHelper::SwitchButton(ICON_FK_LIGHTBULB_O, "Phong Shading",
                              phong_shading);
    ImGuiHelper::ListSeparator();
  }

  {

    static auto index = 0;
    std::vector<const char *> items{"front", "back", "front and back"};
    ImGuiHelper::Comb(ICON_FK_SUN_O, "Lighting", items, index);
    ImGuiHelper::ListSeparator();
  }

  {
    static auto index = 0;
    std::vector<const char *> items{"uniform color", "scalar - f:chart"};
    ImGuiHelper::Comb(ICON_FK_FLASK, "Coloring", items, index);
    ImGuiHelper::ListSeparator();

    if (index == 1) {
      const auto width = ImGui::GetContentRegionAvail().x;
      ImGuiHelper::AlignedText(std::string(ICON_FK_EYEDROPPER) + "    Front",
                               ImGuiHelper::Alignment::kVerticalCenter);
      ImGui::SameLine();
      ImGui::SetCursorPosX(width - layout::kColorWidgetWidth);
      ImGui::PushItemWidth(layout::kColorWidgetWidth);
      static ImColor font_color{
          ImGui::GetStyle().Colors[ImGuiCol_PlotHistogram]};
      ImGui::ColorEdit4("FrontColor##default", (float *)&font_color,
                        ImGuiColorEditFlags_NoLabel);
      ImGuiHelper::ListSeparator();
      ImGui::PopItemWidth();
    }

    {
      const auto width = ImGui::GetContentRegionAvail().x;
      ImGuiHelper::AlignedText(std::string(ICON_FK_EYEDROPPER) + "    Back",
                               ImGuiHelper::Alignment::kVerticalCenter);
      ImGui::SameLine();
      ImGui::SetCursorPosX(width - layout::kColorWidgetWidth);
      ImGui::PushItemWidth(layout::kColorWidgetWidth);
      static ImColor back_color{ImGui::GetStyle().Colors[ImGuiCol_PlotLines]};
      ImGui::ColorEdit4("BackColor##default", (float *)&back_color,
                        ImGuiColorEditFlags_NoLabel);
      ImGuiHelper::ListSeparator();
      ImGui::PopItemWidth();
    }
  }

  {
    static auto index = 0;
    std::vector<const char *> items{"disabled", "f:normal"};
    ImGuiHelper::Comb(ICON_FK_ARROW_CIRCLE_O_UP, "Vecor field", items, index);
  }
}
} // namespace SurfaceRenderOptions