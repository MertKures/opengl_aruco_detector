#include "utils/camera_utils.hpp"

bool is_video_capture_device(const std::string &device_path)
{
    // TODO: Test if it works with O_RDONLY | O_NONBLOCK flags.
    int fd = open(device_path.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd == -1)
        return false;

    v4l2_capability cap{};
    bool result = false;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) != -1)
    {
        if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
        {
            result = true;
        }
    }
    close(fd);
    return result;
}

std::vector<std::string> get_video_devices()
{
    std::vector<std::string> devices;

    devices.push_back("None");

    for (const auto &entry : std::filesystem::directory_iterator("/dev"))
    {
        if (entry.path().string().find("/dev/video") == 0 && is_video_capture_device(entry.path().string()))
        {
            // std::cout << "Camera found: " << entry.path() << std::endl;
            devices.push_back(entry.path().string());
        }
    }

    return devices;
} 