cmake_minimum_required(VERSION 3.10)

project(RenderTests)

add_subdirectory(third_party/fmt)

add_subdirectory(third_party/gflags)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/tinyxml2)

add_subdirectory(tools/wayland-protocol-generator)

file(GLOB_RECURSE SOURCES src/*.cc src/*.h src/*.inl)

add_custom_command(
    OUTPUT src/graphics/wayland/internal/wayland.h
           src/graphics/wayland/internal/wayland.cc
    COMMAND wayland-protocol-generator ARGS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/wayland/protocol/wayland.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/wayland/internal"
)

list(APPEND SOURCES
     "src/graphics/wayland/internal/wayland.cc"
     "src/graphics/wayland/internal/wayland.h")

add_executable(RenderTests ${SOURCES})
target_link_libraries(RenderTests PUBLIC X11 dl fmt wayland-client)
target_include_directories(RenderTests PRIVATE src)
set_target_properties(RenderTests PROPERTIES
                      CXX_STANDARD 20
                      CXX_STANDARD_REQUIRED ON)
target_compile_options(RenderTests PUBLIC
                      "-Wall" "-Wextra" "-Werror" "-Wno-unused-variable" "-Wno-unused-parameter")
