#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include "imgui.h"

class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual void render(ImGuiIO& io) = 0;
};

#endif
