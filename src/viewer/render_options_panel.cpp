#include <array>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "common/singleton.h"
#include "common/swtich.h"
#include "fork_awesome.h"
#include "glm/fwd.hpp"
#include "glm/vec4.hpp"
#include "global_render_options.h"
#include "imgui/imgui.h"
#include "imgui_helper.hpp"
#include "line_render_options.h"
#include "points_render_options.h"
#include "surface_render_options.h"
#include "viewer/layout.h"
#include "viewer/style.h"

namespace RenderOptionsPanel {
void show() {
  ImGuiWindowFlags option_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;
  const auto parent_size = ImGui::GetMainViewport()->WorkSize;
  const auto parent_pos = ImGui::GetMainViewport()->WorkPos;

  static auto file_panel_bottom = 0.0F;
  // file panel
  {
    ImGui::SetNextWindowSize({layout::kRenderOptionsPanelWidth, 0});
    ImGui::SetNextWindowPos({parent_pos.x + layout::kMargin, parent_pos.y + layout::kMargin});
    ImGui::SetNextWindowBgAlpha(0.5F);
    ImGui::Begin("FilePanel", nullptr, option_flags | ImGuiWindowFlags_NoScrollbar);

    ImGuiHelper::AlignedText(std::string(ICON_FK_FILE) + " ", ImGuiHelper::Alignment::kVerticalCenter);
    ImGui::SameLine();

    if (ImGui::Button("Load mesh from file...")) {
      make_singleton<common::Switch>().OpenFileClicked.fire();
    }
    file_panel_bottom = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;

    {
      const auto width = ImGui::GetContentRegionAvailWidth();
      ImGuiHelper::AlignedText(std::string(ICON_FK_EYEDROPPER) + "    MeshColor",
                               ImGuiHelper::Alignment::kVerticalCenter);
      ImGui::SameLine();
      ImGui::SetCursorPosX(width - layout::kColorWidgetWidth);
      ImGui::PushItemWidth(layout::kColorWidgetWidth);
      static ImColor mesh_color{ImGui::GetStyle().Colors[ImGuiCol_PlotLines]};
      if (ImGui::ColorEdit4("MeshColor##default", (float *)&mesh_color, ImGuiColorEditFlags_NoLabel)) {
        make_singleton<common::Switch>().ColorChanged.fire(
            glm::vec4(mesh_color.Value.x, mesh_color.Value.y, mesh_color.Value.z, mesh_color.Value.w));
      }
      ImGuiHelper::ListSeparator();
      ImGui::PopItemWidth();
    }

    ImGui::End();
  }

  // options panel
  static float options_panel_bottom = 0;
  {
    ImGui::SetNextWindowSize({layout::kRenderOptionsPanelWidth, 0});
    ImGui::SetNextWindowPos({parent_pos.x + layout::kMargin, file_panel_bottom + layout::kPanelSpacing});
    ImGui::SetNextWindowBgAlpha(0.5F);
    ImGui::Begin("RenderOptionsWindow", nullptr, option_flags | ImGuiWindowFlags_NoScrollbar);
    // tabs
    static auto tabs = std::vector<std::string>{"Surface", "Line", "Points", "Global"};
    static auto selected_index = 0;
    ImGuiHelper::ButtonTab(tabs, selected_index);

    // detail options
    {
      ImGui::Dummy({0, 10});
      using func = std::function<void()>;
      static std::array<func, 4> options{SurfaceRenderOptions::show, LineRenderOptions::show, PointsRenderOptions::show,
                                         GlobRenderOptions::show};
      options.at(selected_index)();
    }
    options_panel_bottom = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
    ImGui::End();
  }

  // debug panel
  static auto debug_panel_bottom = 0.0F;
  {
    ImGui::SetNextWindowSize({layout::kRenderOptionsPanelWidth, 0});
    ImGui::SetNextWindowPos({parent_pos.x + layout::kMargin, options_panel_bottom + layout::kPanelSpacing});
    ImGui::SetNextWindowBgAlpha(0.5F);
    ImGui::Begin("DebugPanel", nullptr, option_flags | ImGuiWindowFlags_NoScrollbar);

    auto static show_metrics = false;
    ImGuiHelper::SwitchButton(ICON_FK_WRENCH, "Window Metrics", show_metrics);
    ImGuiHelper::ListSeparator();
    if (show_metrics) {
      ImGui::ShowMetricsWindow();
    }

    auto static show_demo = false;
    ImGuiHelper::SwitchButton(ICON_FK_ROCKET, "Demo", show_demo);
    if (show_demo) {
      ImGui::ShowDemoWindow();
    }
    debug_panel_bottom = ImGui::GetWindowSize().y;
    ImGui::End();
  }
}
}  // namespace RenderOptionsPanel