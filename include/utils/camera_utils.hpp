#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H

#include <vector>
#include <filesystem>
#include <iostream>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

bool is_video_capture_device(const std::string &device_path);
std::vector<std::string> get_video_devices();

#endif // CAMERA_UTILS_H