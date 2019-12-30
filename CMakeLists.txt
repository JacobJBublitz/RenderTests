cmake_minimum_required(VERSION 3.10)

project(RenderTests)

option(BUILD_SHARED_LIBS "Build shared instead of static libraries." OFF)

add_subdirectory(third_party/fmt)
add_subdirectory(third_party/gflags)
set(OPTION_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glbinding)
set(gflags_FOUND ON)
add_subdirectory(third_party/glog)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/tinyxml2)

add_subdirectory(tools/wayland-protocol-generator)

add_subdirectory(src)

add_executable(RenderTests ${SOURCES})
target_link_libraries(RenderTests PUBLIC dl fmt gflags glog glbinding ${LINK_LIBRARIES})
target_include_directories(RenderTests PRIVATE src)
set_target_properties(RenderTests PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON)
target_compile_options(RenderTests PUBLIC
                      "-Wall" "-Wextra" "-Werror" "-Wno-unused-variable" "-Wno-unused-parameter")
