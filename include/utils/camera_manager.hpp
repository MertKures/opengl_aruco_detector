#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <chrono>

namespace Utils
{
    class CameraManager
    {
    public:
        CameraManager();
        ~CameraManager();

        // Start the camera thread
        void start();
        
        // Stop the camera thread
        void stop();

        // Set the camera device to use
        void set_camera_device(const std::string& device_path);
        
        // Get the current camera device
        std::string get_camera_device() const;

        // Get the latest frame (thread-safe)
        cv::Mat get_latest_frame() const;
        
        // Check if camera is opened
        bool is_camera_opened() const;
        
        // Get available camera devices
        std::vector<std::string> get_camera_devices() const;

        // Check if the manager is running
        bool is_running() const;

    private:
        // Camera thread function
        void camera_thread_function();
        
        // Update camera devices list
        void update_camera_devices();

        // Member variables
        mutable std::mutex frame_mutex_;
        mutable std::mutex device_mutex_;
        mutable std::mutex devices_list_mutex_;
        
        std::thread camera_thread_;
        std::atomic<bool> running_;
        std::atomic<bool> camera_opened_;
        
        cv::VideoCapture cap_;
        cv::Mat current_frame_;
        std::string current_device_;
        
        std::vector<std::string> camera_devices_;
        std::chrono::steady_clock::time_point last_devices_check_;
    };

} // namespace Utils

#endif // CAMERA_MANAGER_H 