#ifndef ARUCO_WINDOH_H
#define ARUCO_WINDOW_H

#include "ui/iwindow.hpp"

namespace UI
{
    class ArucoWindow : public IWindow
    {
    public:
        ArucoWindow();
        void render(ImGuiIO &io) override;
    };

} // namespace UI

#endif