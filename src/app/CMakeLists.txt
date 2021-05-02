set(app ${CMAKE_PROJECT_NAME})
add_executable(app WIN32 main.cpp)
target_link_libraries(app common viewer)