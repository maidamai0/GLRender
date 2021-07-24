set(app ${CMAKE_PROJECT_NAME})
file(GLOB srcs *.cpp)
if(WIN32)
    list(APPEND srcs res.rc)
endif()

add_executable(${app} WIN32 ${srcs})
target_link_libraries(${app} common viewer mesh render)

add_custom_command(
    TARGET ${app}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:${app}>/models
    COMMENT "Copy models"
)