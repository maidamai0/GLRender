get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB srcs *.cpp)
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
add_library(${lib_name} ${srcs})
target_include_directories(${lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/deps/imgui/imgui)
target_link_libraries(${lib_name} PRIVATE glad glfw imgui)