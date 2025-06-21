#ifndef SCENE_WINDOH_H
#define SCENE_WINDOW_H

#include "ui/iwindow.hpp"
#include "utils/camera_manager.hpp"
#include <opencv2/objdetect/aruco_detector.hpp>
#include <memory>
#include <map>
#include <chrono>

// Forward declaration
namespace UI
{
    class ArucoWindow;
}

namespace UI
{
    class SceneWindow : public IWindow
    {
    public:
        SceneWindow(std::shared_ptr<Utils::CameraManager> camera_manager, 
                   std::shared_ptr<UI::ArucoWindow> aruco_window);
        void render(ImGuiIO &io) override;

    private:
        void process_aruco_detection();
        void render_detected_arucos();
        void render_statistics();
        void reset_statistics();
        void generate_and_upload_image();

    private:
        std::shared_ptr<Utils::CameraManager> camera_manager_;
        std::shared_ptr<UI::ArucoWindow> aruco_window_;
        
        // Image rendering
        cv::Mat current_frame_;
        unsigned int frame_texture_ = 0;
        int frame_width_ = 0;
        int frame_height_ = 0;
        
        // ArUco detection
        cv::aruco::ArucoDetector detector_;
        std::vector<int> detected_marker_ids_;
        std::vector<std::vector<cv::Point2f>> detected_marker_corners_;
        
        // Statistics tracking
        struct MarkerStats {
            int total_detections = 0;
            int current_frame_detections = 0;
            std::chrono::steady_clock::time_point first_detection;
            std::chrono::steady_clock::time_point last_detection;
            double detections_per_second = 0.0;
        };
        
        std::map<int, MarkerStats> marker_statistics_;
        std::chrono::steady_clock::time_point last_stats_update_;
        cv::aruco::PredefinedDictionaryType current_dictionary_type_;
        int current_selected_marker_id_;
        
        // UI state
        ImVec2 window_pos_, window_size_;
    };

} // namespace UI

#endif