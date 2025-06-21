#include "glad/gl.h"
#include "ui/camera_window.hpp"
#include <opencv2/opencv.hpp>

UI::CameraWindow::CameraWindow(std::shared_ptr<Utils::CameraManager> camera_manager)
    : camera_manager_(camera_manager)
{
    camera_manager_->start();
}

UI::CameraWindow::~CameraWindow()
{
    if (camera_manager_)
    {
        camera_manager_->stop();
    }
}

void UI::CameraWindow::render(ImGuiIO &io)
{
    window_size = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y / 2.0);
    window_pos = ImVec2(io.DisplaySize.x / 2.0, 0);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin("Camera window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::SetCursorScreenPos(ImVec2(window_pos.x + window_size.x / 2 - 150, window_pos.y + 30));
    ImGui::SetNextItemWidth(300);

    // Get camera devices from the manager
    auto camera_devices = camera_manager_->get_camera_devices();

    if (ImGui::BeginCombo("##CameraComboBox", selected_camera_device.c_str()))
    {
        for (const std::string &camera_path : camera_devices)
        {
            bool is_selected = camera_path == selected_camera_device;
            if (ImGui::Selectable(camera_path.c_str(), is_selected))
            {
                selected_camera_device = camera_path;
                camera_manager_->set_camera_device(camera_path);
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Display camera frame if available
    if (camera_manager_->is_camera_opened())
    {
        cv::Mat frame = camera_manager_->get_latest_frame();

        if (!frame.empty())
        {
            // Update the image data
            image_rgba = frame;
            generate_and_upload_image();

            float original_aspect_ratio = static_cast<float>(image_rgba.cols) / image_rgba.rows;
            float max_width = window_size.x * 0.8f;
            float max_height = window_size.y * 0.6f;

            ImVec2 image_size;
            if (max_width / original_aspect_ratio <= max_height)
                image_size = ImVec2(max_width, max_width / original_aspect_ratio);
            else
                image_size = ImVec2(max_height * original_aspect_ratio, max_height);

            ImGui::SetCursorScreenPos(
                ImVec2(
                    window_pos.x + window_size.x / 2 - image_size.x / 2,
                    window_pos.y + window_size.y / 2 - image_size.y / 2 + 10));

            ImGui::Image(image_texture, image_size);
        }
    }

    ImGui::End();
}

void UI::CameraWindow::generate_and_upload_image()
{
    if (image_texture == 0 || image_width != image_rgba.cols || image_height != image_rgba.rows)
    {
        if (image_texture != 0)
        {
            glDeleteTextures(1, &image_texture);
        }

        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_rgba.cols, image_rgba.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        image_width = image_rgba.cols;
        image_height = image_rgba.rows;
    }

    glBindTexture(GL_TEXTURE_2D, image_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_rgba.cols, image_rgba.rows, GL_RGBA, GL_UNSIGNED_BYTE, image_rgba.data);
}
