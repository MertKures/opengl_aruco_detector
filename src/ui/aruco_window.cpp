#include "ui/aruco_window.hpp"
#include <opencv2/imgproc.hpp>
#include "glad/gl.h"
#include <iostream>

UI::ArucoWindow::ArucoWindow()
{
    int markerId = 0;
    int maxId = aruco_dicts[current_item].max_id;
    generate_and_upload_marker(aruco_dicts[current_item].type, markerId, maxId);
    previous_item = current_item;
}

void UI::ArucoWindow::render(ImGuiIO &io)
{
    window_size = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y / 2.0);
    window_pos = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y / 2.0);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin("Aruco selection window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Aruco markers");
    ImGui::SameLine();

    if (ImGui::BeginCombo("##ArucoMarkersToChoose", aruco_dicts[current_item].name))
    {
        for (int index = 0; index < sizeof(aruco_dicts) / sizeof(*aruco_dicts); index++)
        {
            bool is_selected = current_item == index;
            if (ImGui::Selectable(aruco_dicts[index].name, is_selected))
            {
                current_item = index;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Marker ID: ");
    ImGui::SameLine();
    ImGui::InputInt("##MarkerIDInput", &selected_marker_id, 0, 0);

    if (current_item != previous_item)
    {
        generate_and_upload_marker(aruco_dicts[current_item].type, selected_marker_id, aruco_dicts[current_item].max_id);
        previous_item = current_item;
    }

    if (marker_texture != 0)
    {
        float left = ImGui::GetWindowContentRegionMax().x / 2 - 100;

        if (left < 0)
        {
            left = 0;
        }

        ImVec2 image_size(200, 200);

        ImGui::SetCursorPosX(left);
        ImGui::Image(marker_texture, image_size);
    }

    ImGui::End();
}

cv::aruco::PredefinedDictionaryType UI::ArucoWindow::get_current_dictionary_type() const
{
    return aruco_dicts[current_item].type;
}

int UI::ArucoWindow::get_selected_marker_id() const
{
    return selected_marker_id;
}

int UI::ArucoWindow::get_current_item() const
{
    return current_item;
}

void UI::ArucoWindow::generate_and_upload_marker(cv::aruco::PredefinedDictionaryType dict_type, int marker_id, int max_id)
{
    int markerId = marker_id;

    if (markerId >= max_id)
        markerId = max_id - 1;

    // TODO: Make it customizable.
    const int markerSize = 200;
    const int borderBits = 1;

    dictionary = cv::aruco::getPredefinedDictionary(dict_type);
    cv::aruco::generateImageMarker(dictionary, markerId, markerSize, marker_image, borderBits);

    cv::Mat rgba_image;
    cv::cvtColor(marker_image, rgba_image, cv::COLOR_GRAY2RGBA);

    if (marker_texture == 0)
        glGenTextures(1, &marker_texture);

    glBindTexture(GL_TEXTURE_2D, marker_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba_image.cols, rgba_image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_image.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture_uploaded = true;
}
