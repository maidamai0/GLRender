cmake_minimum_required(VERSION 3.16)

get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${ROOT} C CXX)

set(CMAKE_GENERATOR Ninja)

# cpp compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
  # add_compile_options(/fsanitize=address)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  add_compile_options(/Zc:__cplusplus) # see https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-160 for detail

  # win10 or later, see https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-160
  add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_program(GIT_CHGLOG git-chglog)

if(GIT_CHGLOG)
  message(STATUS "Found git-chglog:${GIT_CHGLOG}")
  execute_process(
    COMMAND ${GIT_CHGLOG}
    OUTPUT_FILE CHANGELOG.md
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
else()
  message(STATUS "git-chglog not found, changelog will not be updated")
endif()

# definitions
add_compile_definitions(APP_NAME="${CMAKE_PROJECT_NAME}")

# setup dependencies
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)
add_subdirectory(deps)
link_libraries(fmt::fmt-header-only)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/doctest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/spdlog/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/happly)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/signal_slot)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/nativefiledialog_extended/src/include)

add_subdirectory(src)