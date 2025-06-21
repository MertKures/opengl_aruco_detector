#include <iostream>
#include "ui/scene_window.hpp"
#include "ui/aruco_window.hpp"
#include "glad/gl.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>

UI::SceneWindow::SceneWindow(std::shared_ptr<Utils::CameraManager> camera_manager,
                             std::shared_ptr<UI::ArucoWindow> aruco_window)
    : camera_manager_(camera_manager), aruco_window_(aruco_window), current_dictionary_type_(cv::aruco::DICT_4X4_50), current_selected_marker_id_(0), last_stats_update_(std::chrono::steady_clock::now())
{
    // Initialize detector with default dictionary
    detector_ = cv::aruco::ArucoDetector(cv::aruco::getPredefinedDictionary(current_dictionary_type_));
}

void UI::SceneWindow::render(ImGuiIO &io)
{
    window_size = ImVec2(io.DisplaySize.x / 2.0, io.DisplaySize.y);
    window_pos = ImVec2(0, 0);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin("Scene window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Process ArUco detection if camera is available
    if (camera_manager_ && camera_manager_->is_camera_opened())
    {
        process_aruco_detection();
        render_detected_arucos();
        render_statistics();
    }
    else
    {
        ImGui::Text("No camera available for ArUco detection");
    }

    ImGui::End();
}

void UI::SceneWindow::process_aruco_detection()
{
    // Get latest frame from camera
    cv::Mat frame = camera_manager_->get_latest_frame();
    if (frame.empty())
        return;

    // Check if ArUco settings have changed
    if (aruco_window_)
    {
        auto new_dictionary_type = aruco_window_->get_current_dictionary_type();
        auto new_selected_marker_id = aruco_window_->get_selected_marker_id();

        // If settings changed, reset statistics and update detector
        bool settings_changed = false;
        if (new_dictionary_type != current_dictionary_type_)
        {
            current_dictionary_type_ = new_dictionary_type;
            detector_ = cv::aruco::ArucoDetector(cv::aruco::getPredefinedDictionary(current_dictionary_type_));
            settings_changed = true;
        }

        if (new_selected_marker_id != current_selected_marker_id_)
        {
            current_selected_marker_id_ = new_selected_marker_id;
            settings_changed = true;
        }

        if (settings_changed)
        {
            reset_statistics();
        }
    }

    // Convert RGBA to grayscale for ArUco detection
    cv::Mat gray_frame;
    cv::cvtColor(frame, gray_frame, cv::COLOR_RGBA2GRAY);

    // Detect ArUco markers
    detector_.detectMarkers(gray_frame, detected_marker_corners_, detected_marker_ids_);

    // Update statistics
    auto now = std::chrono::steady_clock::now();

    // Clear current frame detections
    for (auto &stat : marker_statistics_)
    {
        stat.second.current_frame_detections = 0;
    }

    // Update detection counts
    for (int marker_id : detected_marker_ids_)
    {
        auto &stat = marker_statistics_[marker_id];
        stat.total_detections++;
        stat.current_frame_detections++;
        stat.last_detection = now;

        if (stat.total_detections == 1)
        {
            stat.first_detection = now;
        }
    }

    // Update detections per second every second
    if (now - last_stats_update_ >= std::chrono::seconds(1))
    {
        for (auto &stat : marker_statistics_)
        {
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                                stat.second.last_detection - stat.second.first_detection)
                                .count();

            if (duration > 0)
            {
                stat.second.detections_per_second = static_cast<double>(stat.second.total_detections) / duration;
            }
        }
        last_stats_update_ = now;
    }

    // Store frame for rendering
    current_frame_ = frame.clone();
}

void UI::SceneWindow::render_detected_arucos()
{
    if (current_frame_.empty())
        return;

    // Create a copy of the frame for drawing
    cv::Mat display_frame = current_frame_.clone();

    // Convert RGBA to BGR for OpenCV drawing
    if (display_frame.channels() == 4)
    {
        cv::cvtColor(display_frame, display_frame, cv::COLOR_RGBA2BGR);
    }

    // Draw detected ArUco markers
    if (!detected_marker_ids_.empty())
    {
        cv::aruco::drawDetectedMarkers(display_frame, detected_marker_corners_, detected_marker_ids_);

        // Draw additional information for each marker
        for (size_t i = 0; i < detected_marker_ids_.size(); i++)
        {
            int marker_id = detected_marker_ids_[i];
            const auto &corners = detected_marker_corners_[i];

            // Calculate center of the marker
            cv::Point2f center(0, 0);
            for (const auto &corner : corners)
            {
                center += corner;
            }
            center /= 4.0f;

            // Draw marker ID
            std::string id_text = "ID: " + std::to_string(marker_id);
            cv::putText(display_frame, id_text, center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        }
    }

    // Convert back to RGBA for OpenGL upload
    cv::Mat display_frame_rgba;
    cv::cvtColor(display_frame, display_frame_rgba, cv::COLOR_BGR2RGBA);

    // Upload to OpenGL texture
    if (frame_texture_ == 0 || frame_width_ != display_frame_rgba.cols || frame_height_ != display_frame_rgba.rows)
    {
        if (frame_texture_ != 0)
        {
            glDeleteTextures(1, &frame_texture_);
        }

        glGenTextures(1, &frame_texture_);
        glBindTexture(GL_TEXTURE_2D, frame_texture_);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, display_frame_rgba.cols, display_frame_rgba.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        frame_width_ = display_frame_rgba.cols;
        frame_height_ = display_frame_rgba.rows;
    }

    glBindTexture(GL_TEXTURE_2D, frame_texture_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, display_frame_rgba.cols, display_frame_rgba.rows, GL_RGBA, GL_UNSIGNED_BYTE, display_frame_rgba.data);

    // Display the image
    ImVec2 image_size(window_size.x / 1.5, window_size.y / 1.5);
    ImGui::SetCursorScreenPos(
        ImVec2(
            window_pos.x + window_size.x / 2 - image_size.x / 2,
            window_pos.y + 10));
    ImGui::Image(frame_texture_, image_size);
}

void UI::SceneWindow::render_statistics()
{
    ImGui::Separator();
    ImGui::Text("ArUco Detection Statistics:");

    if (marker_statistics_.empty())
    {
        ImGui::Text("No markers detected yet");
        return;
    }

    for (const auto &stat_pair : marker_statistics_)
    {
        int marker_id = stat_pair.first;
        const auto &stat = stat_pair.second;

        ImGui::Text("Marker ID %d:", marker_id);
        ImGui::Indent();
        ImGui::Text("Total detections: %d", stat.total_detections);
        ImGui::Text("Current frame: %s", stat.current_frame_detections > 0 ? "Detected" : "Not detected");
        ImGui::Text("Detections per second: %.2f", stat.detections_per_second);

        if (stat.total_detections > 0)
        {
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(
                                stat.last_detection - stat.first_detection)
                                .count();
            ImGui::Text("Detection duration: %ld seconds", duration);
        }
        ImGui::Unindent();
        ImGui::Spacing();
    }
}

void UI::SceneWindow::reset_statistics()
{
    marker_statistics_.clear();
    last_stats_update_ = std::chrono::steady_clock::now();
}