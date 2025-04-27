#ifndef MAIN_WINDOH_H
#define MAIN_WINDOW_H

#include "ui/iwindow.hpp"

namespace UI
{
    class MainWindow : public IWindow
    {
    public:
        MainWindow();
        void render(ImGuiIO& io) override;

    private:
        ImVec4 clear_color;
    };

} // namespace UI

#endif