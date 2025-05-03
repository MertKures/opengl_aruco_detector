#include "ui/main_window.hpp"
#include <iostream>
#include <imgui_internal.h>
#include "glad/gl.h"
#include "GLFW/glfw3.h"

UI::MainWindow::MainWindow()
{
    window_pos = {0, 0};
    window_size = {0, 0};
}

void UI::MainWindow::render(ImGuiIO &io)
{
    window_size = io.DisplaySize;
    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);
    ImGui::Begin("Main window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);

    for (auto &window : child_windows)
    {
        window->render(io);
    }

    ImGui::End();
}

void UI::MainWindow::add_child_window(std::shared_ptr<IWindow> window)
{
    child_windows.push_back(window);
}