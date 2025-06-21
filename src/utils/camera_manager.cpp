#include "utils/camera_manager.hpp"
#include "utils/camera_utils.hpp"
#include <iostream>

namespace Utils
{
    CameraManager::CameraManager()
        : running_(false), camera_opened_(false), current_device_("None"), last_devices_check_(std::chrono::steady_clock::now())
    {
        update_camera_devices();
    }

    CameraManager::~CameraManager()
    {
        stop();
    }

    void CameraManager::start()
    {
        if (!running_.load())
        {
            running_.store(true);
            camera_thread_ = std::thread(&CameraManager::camera_thread_function, this);
        }
    }

    void CameraManager::stop()
    {
        if (running_.load())
        {
            running_.store(false);

            if (camera_thread_.joinable())
            {
                camera_thread_.join();
            }
        }
    }

    void CameraManager::set_camera_device(const std::string &device_path)
    {
        std::lock_guard<std::mutex> lock(device_mutex_);
        current_device_ = device_path;
    }

    std::string CameraManager::get_camera_device() const
    {
        std::lock_guard<std::mutex> lock(device_mutex_);
        return current_device_;
    }

    cv::Mat CameraManager::get_latest_frame() const
    {
        std::lock_guard<std::mutex> lock(frame_mutex_);
        return current_frame_.clone();
    }

    bool CameraManager::is_camera_opened() const
    {
        return camera_opened_.load();
    }

    std::vector<std::string> CameraManager::get_camera_devices() const
    {
        std::lock_guard<std::mutex> lock(devices_list_mutex_);
        return camera_devices_;
    }

    bool CameraManager::is_running() const
    {
        return running_.load();
    }

    void CameraManager::camera_thread_function()
    {
        const std::chrono::seconds devices_check_interval(5);
        const std::chrono::milliseconds frame_interval(33); // ~30 FPS

        std::string last_device = "None";

        while (running_.load())
        {
            auto now = std::chrono::steady_clock::now();

            // Update camera devices list periodically
            if (now - last_devices_check_ >= devices_check_interval)
            {
                update_camera_devices();
                last_devices_check_ = now;
            }

            // Handle camera operations
            std::string current_device;
            {
                std::lock_guard<std::mutex> lock(device_mutex_);
                current_device = current_device_;
            }

            // Open camera if device changed
            if (current_device != "None" && current_device != last_device)
            {
                cap_.release();
                cap_ = cv::VideoCapture(current_device);

                if (cap_.isOpened())
                {
                    // Set camera properties for better performance
                    cap_.set(cv::CAP_PROP_FRAME_WIDTH, 640);
                    cap_.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
                    cap_.set(cv::CAP_PROP_FPS, 30);
                    cap_.set(cv::CAP_PROP_BUFFERSIZE, 1); // Minimize latency

                    camera_opened_.store(true);
                    last_device = current_device;
                    std::cout << "Camera opened: " << current_device << std::endl;
                }
                else
                {
                    camera_opened_.store(false);
                    std::cout << "Failed to open camera: " << current_device << std::endl;

                    // Reset device selection on failure
                    std::lock_guard<std::mutex> lock(device_mutex_);
                    current_device_ = "None";
                    last_device = "None";
                }
            }
            else if (current_device == "None" && last_device != "None")
            {
                // Close camera if device set to None
                cap_.release();
                camera_opened_.store(false);
                last_device = "None";
                std::cout << "Camera closed" << std::endl;
            }

            // Capture frame if camera is opened
            if (camera_opened_.load() && cap_.isOpened())
            {
                cv::Mat frame_bgr;
                if (cap_.grab() && cap_.retrieve(frame_bgr))
                {
                    if (!frame_bgr.empty())
                    {
                        // Convert to RGBA
                        cv::Mat frame_rgba;
                        cv::cvtColor(frame_bgr, frame_rgba, cv::COLOR_BGR2RGBA);

                        // Update current frame
                        {
                            std::lock_guard<std::mutex> lock(frame_mutex_);
                            current_frame_ = frame_rgba.clone();
                        }
                    }
                }
                else
                {
                    // Frame capture failed, try to reopen camera
                    std::cout << "Frame capture failed, attempting to reopen camera..." << std::endl;
                    cap_.release();
                    camera_opened_.store(false);
                    last_device = "None";
                }
            }

            // Sleep to maintain frame rate
            std::this_thread::sleep_for(frame_interval);
        }

        // Cleanup
        cap_.release();
        camera_opened_.store(false);
        std::cout << "Camera thread stopped" << std::endl;
    }

    void CameraManager::update_camera_devices()
    {
        std::lock_guard<std::mutex> lock(devices_list_mutex_);
        camera_devices_ = get_video_devices();
    }

} // namespace Utils