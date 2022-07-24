#pragma once

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace ImGuiHelper {

enum class Alignment : unsigned char {
  kHorizontalCenter = 1 << 0,
  kVerticalCenter = 1 << 1,
  kCenter = kHorizontalCenter | kVerticalCenter,
};

/**
 * @brief Render text with alignment
 */
inline void AlignedText(const std::string &text, Alignment align,
                        const float &width = 0.0F) {
  const auto alignment = static_cast<unsigned char>(align);
  const auto text_size = ImGui::CalcTextSize(text.c_str());
  const auto wind_size = ImGui::GetContentRegionAvail();
  if (alignment & static_cast<unsigned char>(Alignment::kHorizontalCenter)) {
    if (width < 0.1F) {
      ImGui::SetCursorPosX((wind_size.x - text_size.x) * 0.5F);
    } else {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                           (width - text_size.x) * 0.5F);
    }
  }
  if (alignment & static_cast<unsigned char>(Alignment::kVerticalCenter)) {
    ImGui::AlignTextToFramePadding();
  }

  ImGui::TextUnformatted(text.c_str());
}

inline auto CheckButton(const std::string &label, bool checked,
                        const ImVec2 &size) -> bool {
  if (checked) {
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
    ImGui::PushStyleColor(ImGuiCol_Button,
                          ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
  } else {
    ImGui::PushStyleColor(
        ImGuiCol_ButtonHovered,
        ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive]);
    ImGui::PushStyleColor(ImGuiCol_Button,
                          ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive]);
  }
  if (ImGui::Button(label.c_str(), size)) {
    checked = !checked;
  }

  ImGui::PopStyleColor(2);

  return checked;
}

inline auto ButtonTab(std::vector<std::string> &tabs, int &index) -> int {
  auto checked = 1 << index;
  std::string tab_names;
  std::for_each(tabs.begin(), tabs.end(),
                [&tab_names](const auto item) { tab_names += item; });
  const auto tab_width = ImGui::GetContentRegionAvail().x;
  const auto tab_btn_width = tab_width / static_cast<float>(tabs.size());
  const auto h = ImGui::CalcTextSize(tab_names.c_str()).y;
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, h);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, h);

  ImGui::PushStyleColor(ImGuiCol_ChildBg,
                        ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive]);
  ImGui::BeginChild(tab_names.c_str(),
                    {tab_width, h + ImGui::GetStyle().FramePadding.y * 2},
                    false,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoResize);

  for (int i = 0; i < tabs.size(); ++i) {
    auto &tab = tabs[i];

    // if current tab is checkd, uncheck otheres
    if (CheckButton(tab, checked & (1 << i), ImVec2{tab_btn_width, 0})) {
      checked = 0;
      checked = 1 << i;
    }

    if (i != tabs.size() - 1) {
      ImGui::SameLine();
    }
  }
  ImGui::PopStyleColor();
  ImGui::PopStyleVar(3);
  ImGui::EndChild();

  index = 0;
  while (checked / 2) {
    checked = checked / 2;
    ++index;
  }

  return index;
}

inline void SwitchButton(std::string &&icon, std::string &&label,
                         bool &checked) {
  float height = ImGui::GetFrameHeight();
  float width = height * 1.55F;
  float radius = height * 0.50F;
  const auto frame_width = ImGui::GetContentRegionAvail().x;

  AlignedText(icon + "    " + label, Alignment::kVerticalCenter);
  ImGui::SameLine();

  ImGui::SetCursorPosX(frame_width - width);
  ImVec2 pos = ImGui::GetCursorScreenPos();
  if (ImGui::InvisibleButton(label.c_str(), ImVec2(width, height))) {
    checked = !checked;
  }
  ImU32 col_bg = 0;
  if (checked) {
    col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
  } else {
    col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
  }
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), col_bg,
                           radius);
  draw_list->AddCircleFilled(
      ImVec2(checked ? (pos.x + width - radius) : (pos.x + radius),
             pos.y + radius),
      radius - 1.5F, IM_COL32_WHITE);
}

inline void Comb(std::string &&icon, std::string &&label,
                 const std::vector<const char *> &items, int &index) {
  const auto p_w = ImGui::GetContentRegionAvail().x;
  AlignedText(icon + "    " + label, Alignment::kVerticalCenter);
  ImGui::SameLine();
  ImGui::SetCursorPosX(p_w - 150.0F - ImGui::GetStyle().FramePadding.x);
  ImGui::SetNextItemWidth(150.0F);
  ImGui::Combo((std::string("##") + label).c_str(), &index, items.data(), static_cast<int>(items.size()));
}

inline void InputInt(std::string &&icon, std::string &&label, int &value) {
  const auto p_w = ImGui::GetContentRegionAvail().x;
  AlignedText(icon + "    " + label, Alignment::kVerticalCenter);
  ImGui::SameLine();
  ImGui::SetCursorPosX(p_w - 100.0F - ImGui::GetStyle().FramePadding.x);
  ImGui::SetNextItemWidth(100.0F);
  ImGui::InputInt((std::string("##") + label).c_str(), &value);
}

inline void ListSeparator(float indent = 30.0F) {
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  if (window->SkipItems) {
    return;
  }

  ImGuiContext &g = *GImGui;

  float thickness_draw = 1.0F;
  float thickness_layout = 0.0F;
  // Horizontal Separator
  float x1 = window->Pos.x + indent;
  float x2 = window->Pos.x + window->Size.x;

  // FIXME-WORKRECT: old hack (#205) until we decide of consistent behavior with WorkRect/Indent and Separator
  if (g.GroupStack.Size > 0 && g.GroupStack.back().WindowID == window->ID) {
    x1 += window->DC.Indent.x;
  }

  // We don't provide our width to the layout so that it doesn't get feed back into AutoFit
  const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + thickness_draw));
  ImGui::ItemSize(ImVec2(0.0F, thickness_layout));
  const bool item_visible = ImGui::ItemAdd(bb, 0);
  if (item_visible) {
    // Draw
    window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), ImGui::GetColorU32(ImGuiCol_Separator));
  }
}

}  // namespace ImGuiHelper