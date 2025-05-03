#ifndef SCENE_WINDOH_H
#define SCENE_WINDOW_H

#include "ui/iwindow.hpp"

namespace UI
{
    class SceneWindow : public IWindow
    {
    public:
        SceneWindow();
        void render(ImGuiIO &io) override;
    };

} // namespace UI

#endif