#include <iostream>
#include <stdio.h>
#include <thread>
#include <chrono>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "main.hpp"
#include "ui/main_window.hpp"
#include "ui/scene_window.hpp"
#include "ui/camera_window.hpp"
#include "ui/aruco_window.hpp"

static void glfw_error_callback(int error, const char *description)
{
    std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int argc, char **argv)
{
    const std::string keys =
        "{help h |      | Print a text for help.}"
        "{fps    | 30.0 | Frames per second.    }";

    cv::CommandLineParser parser = cv::CommandLineParser(argc, argv, keys);

    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }
    else if (parser.has("help") || parser.has("h"))
    {
        parser.printMessage();
        return 0;
    }

    assert(parser.get<float>("fps") > 0.0);
    const unsigned short LOOP_DELAY = static_cast<unsigned short>(1.0 / parser.get<float>("fps") * 1000);

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        return 1;
    }

    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);

    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context\n"
                  << std::endl;
        return -1;
    }

    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    UI::MainWindow imgui_main_window;
    std::shared_ptr<UI::SceneWindow> scene_window = std::make_shared<UI::SceneWindow>();
    std::shared_ptr<UI::CameraWindow> camera_window = std::make_shared<UI::CameraWindow>();
    std::shared_ptr<UI::ArucoWindow> aruco_window = std::make_shared<UI::ArucoWindow>();

    imgui_main_window.add_child_window(scene_window);
    imgui_main_window.add_child_window(camera_window);
    imgui_main_window.add_child_window(aruco_window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            std::this_thread::sleep_for(std::chrono::nanoseconds(LOOP_DELAY));
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        imgui_main_window.render(io);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
