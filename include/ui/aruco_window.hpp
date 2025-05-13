#ifndef ARUCO_WINDOH_H
#define ARUCO_WINDOW_H

#include "ui/iwindow.hpp"
#include <opencv2/objdetect/aruco_detector.hpp>

struct DictionaryEntry
{
    const char *name;
    cv::aruco::PredefinedDictionaryType type;
    int max_id; // exclusive upper bound (e.g., 50 means valid ids are 0–49)
};

namespace UI
{
    class ArucoWindow : public IWindow
    {
    public:
        ArucoWindow();
        void render(ImGuiIO &io) override;
        void generate_and_upload_marker(cv::aruco::PredefinedDictionaryType dict_type, int marker_id, int max_id);

        int current_item = 0, previous_item = -1, selected_marker_id = 0;

    private:
        DictionaryEntry aruco_dicts[21] = {
            {"4x4_50", cv::aruco::DICT_4X4_50, 50},
            {"4x4_100", cv::aruco::DICT_4X4_100, 100},
            {"4x4_250", cv::aruco::DICT_4X4_250, 250},
            {"4x4_1000", cv::aruco::DICT_4X4_1000, 1000},
            {"5x5_50", cv::aruco::DICT_5X5_50, 50},
            {"5x5_100", cv::aruco::DICT_5X5_100, 100},
            {"5x5_250", cv::aruco::DICT_5X5_250, 250},
            {"5x5_1000", cv::aruco::DICT_5X5_1000, 1000},
            {"6x6_50", cv::aruco::DICT_6X6_50, 50},
            {"6x6_100", cv::aruco::DICT_6X6_100, 100},
            {"6x6_250", cv::aruco::DICT_6X6_250, 250},
            {"6x6_1000", cv::aruco::DICT_6X6_1000, 1000},
            {"7x7_50", cv::aruco::DICT_7X7_50, 50},
            {"7x7_100", cv::aruco::DICT_7X7_100, 100},
            {"7x7_250", cv::aruco::DICT_7X7_250, 250},
            {"7x7_1000", cv::aruco::DICT_7X7_1000, 1000},
            {"Aruco Original", cv::aruco::DICT_ARUCO_ORIGINAL, 1024}, // arbitrary upper bound
            {"AprilTag 16h5", cv::aruco::DICT_APRILTAG_16h5, 30},
            {"AprilTag 25h9", cv::aruco::DICT_APRILTAG_25h9, 35},
            {"AprilTag 36h10", cv::aruco::DICT_APRILTAG_36h10, 2320},
            {"AprilTag 36h11", cv::aruco::DICT_APRILTAG_36h11, 587}};
    };
}; // namespace UI

#endif