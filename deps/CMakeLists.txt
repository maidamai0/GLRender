add_subdirectory(glad)
add_subdirectory(fmt)
add_subdirectory(glfw)
add_subdirectory(glm)
set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "use external fmt" FORCE)
add_subdirectory(spdlog)
add_subdirectory(imgui)
add_subdirectory(nativefiledialog_extended)
add_subdirectory(sigslot)