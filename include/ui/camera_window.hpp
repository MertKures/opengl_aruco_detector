#ifndef CAMERA_WINDOH_H
#define CAMERA_WINDOW_H

#include "ui/iwindow.hpp"

namespace UI
{
    class CameraWindow : public IWindow
    {
    public:
        CameraWindow();
        void render(ImGuiIO &io) override;
    };

} // namespace UI

#endif