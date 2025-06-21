#ifndef CAMERA_WINDOH_H
#define CAMERA_WINDOW_H

#include "ui/iwindow.hpp"
#include "utils/camera_manager.hpp"
#include <string>
#include <chrono>
#include <vector>
#include <opencv2/opencv.hpp>
#include <memory>

namespace UI
{
    class CameraWindow : public IWindow
    {
    public:
        CameraWindow();
        ~CameraWindow();
        void render(ImGuiIO &io) override;

    private:
        void generate_and_upload_image();

    public:
        std::string selected_camera_device = "None";

        cv::Mat image_rgba;
        int image_width = 0;
        int image_height = 0;
        unsigned int image_texture = 0;
        bool texture_uploaded = false;

        ImVec2 window_pos, window_size;

    private:
        std::unique_ptr<Utils::CameraManager> camera_manager_;
    };

} // namespace UI

#endif