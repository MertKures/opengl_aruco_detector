#ifndef MAIN_WINDOH_H
#define MAIN_WINDOW_H

#include <vector>
#include <memory>
#include "ui/iwindow.hpp"

namespace UI
{
    class MainWindow : public IWindow
    {
    public:
        MainWindow();
        void render(ImGuiIO& io) override;
        void add_child_window(std::shared_ptr<IWindow> window);

    private:
        ImVec4 clear_color;
        std::vector<std::shared_ptr<IWindow>> child_windows;
    };

} // namespace UI

#endif