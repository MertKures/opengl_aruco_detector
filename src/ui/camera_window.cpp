#include "ui/camera_window.hpp"

UI::CameraWindow::CameraWindow()
{
}

void UI::CameraWindow::render(ImGuiIO &io)
{
    window_size = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y / 2.0);
    window_pos = ImVec2(io.DisplaySize.x / 2.0, 0);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::BeginChild("Camera window");
    ImGui::Text("Text !!");
    ImGui::SameLine();
    ImGui::Text("Text !!");
    ImGui::EndChild();
}