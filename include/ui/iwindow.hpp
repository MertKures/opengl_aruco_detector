#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include "imgui.h"

class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual void render(ImGuiIO& io) = 0;
protected:
    ImVec2 window_size;
    ImVec2 window_pos;
};

#endif
