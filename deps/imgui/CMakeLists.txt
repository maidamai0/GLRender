set(srcs
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui ${srcs})
target_include_directories(imgui PUBLIC imgui ${CMAKE_CURRENT_SOURCE_DIR}/../glad/include/)
target_link_libraries(imgui PUBLIC glad glfw)

if (MSVC)
    target_compile_definitions(imgui PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_DEPRECATE
        )
endif()

# add_executable(example imgui/examples/example_glfw_opengl3/main.cpp)
# target_link_libraries(example PRIVATE imgui)