set(app ${CMAKE_PROJECT_NAME})
file(GLOB srcs *.cpp)
if(WIN32)
    list(APPEND srcs res.rc)
endif()

add_executable(${app} WIN32 ${srcs})
target_link_libraries(${app} common viewer)