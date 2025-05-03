#include <iostream>
#include "ui/scene_window.hpp"

UI::SceneWindow::SceneWindow()
{
}

void UI::SceneWindow::render(ImGuiIO &io)
{
    window_size = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y);
    window_pos = ImVec2(0, 0);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::BeginChild("Scene window");

    ImGui::Text("Text !!");
    ImGui::SameLine();
    ImGui::Text("Text !!");
    ImGui::EndChild();
}