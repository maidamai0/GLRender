get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB srcs *.cpp)
add_library(${lib_name} ${srcs})
target_link_libraries(${lib_name} common)