#include <array>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "fork_awesome.h"
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
  ImGui::SetNextWindowSize({layout::kRenderOptionsPanelWidth, 0});
  ImGui::SetNextWindowPos({parent_pos.x + parent_size.x - layout::kRenderOptionsPanelWidth - layout::kMargin,
                           parent_pos.y + layout::kMargin});
  ImGui::SetNextWindowBgAlpha(0.5f);
  static float render_options_height = 0;

  // options panel
  {
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
      options[selected_index]();
    }
    render_options_height = ImGui::GetWindowSize().y;
    ImGui::End();
  }

  // debug panel
  {
    ImGui::SetNextWindowSize({layout::kRenderOptionsPanelWidth, 0});
    ImGui::SetNextWindowPos({parent_pos.x + parent_size.x - layout::kRenderOptionsPanelWidth - layout::kMargin,
                             render_options_height + 20});
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("DebugPanel", nullptr, option_flags | ImGuiWindowFlags_NoScrollbar);

    auto static show_metrics = false;
    ImGuiHelper::SwitchButton(ICON_FK_WRENCH, "Window Metrics", show_metrics);
    ImGui::Separator();
    if (show_metrics) {
      ImGui::ShowMetricsWindow();
    }

    auto static show_demo = false;
    ImGuiHelper::SwitchButton(ICON_FK_ROCKET, "Demo", show_demo);
    // ImGui::Separator();
    if (show_demo) {
      ImGui::ShowDemoWindow();
    }
    ImGui::End();
  }
}
}  // namespace RenderOptionsPanel