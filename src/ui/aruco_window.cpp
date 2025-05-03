#include "ui/aruco_window.hpp"

UI::ArucoWindow::ArucoWindow()
{
}

void UI::ArucoWindow::render(ImGuiIO &io)
{
    window_size = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y / 2.0);
    window_pos = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y / 2.0);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::BeginChild("Aruco window");
    ImGui::Text("Text !!");
    ImGui::SameLine();
    ImGui::Text("Text !!");
    ImGui::EndChild();
}