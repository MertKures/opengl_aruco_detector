add_executable(${PROJECT_NAME}
    main.cpp
    ui/main_window.cpp
    ui/scene_window.cpp
    ui/camera_window.cpp
    ui/aruco_window.cpp
    utils/camera_manager.cpp
    utils/camera_utils.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glad imgui ${OpenCV_LIBS} glm)