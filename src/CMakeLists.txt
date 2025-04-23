add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glad imgui)