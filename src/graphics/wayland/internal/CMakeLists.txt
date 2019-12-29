add_library(wayland-protocol-core
            core.cc core.h
            util.h)
set_target_properties(wayland-protocol-core PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON)
target_include_directories(wayland-protocol-core PUBLIC
                           "${RenderTests_SOURCE_DIR}/src")
target_link_libraries(wayland-protocol-core PUBLIC
                      wayland-client)     

add_custom_command(
     OUTPUT "wayland.cc"
            "wayland.h"
     COMMAND wayland-protocol-generator ARGS "${RenderTests_SOURCE_DIR}/third_party/wayland/protocol/wayland.xml"
     DEPENDS wayland-protocol-generator
)

add_library(wayland-protocol-base STATIC wayland.cc wayland.h)
set_target_properties(wayland-protocol-base PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON)
target_include_directories(wayland-protocol-base PUBLIC
                           "${RenderTests_BINARY_DIR}/src")
target_link_libraries(wayland-protocol-base PUBLIC
                      wayland-protocol-core)

add_custom_command(
     OUTPUT "xdg_shell.cc"
            "xdg_shell.h"
     COMMAND wayland-protocol-generator
     ARGS    "${RenderTests_SOURCE_DIR}/third_party/wayland-protocols/stable/xdg-shell/xdg-shell.xml"
             "${RenderTests_SOURCE_DIR}/third_party/wayland/protocol/wayland.xml"
     DEPENDS wayland-protocol-generator
)

add_library(wayland-protocol-xdg_shell STATIC xdg_shell.cc xdg_shell.h)
set_target_properties(wayland-protocol-xdg_shell PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON)
target_include_directories(wayland-protocol-xdg_shell PUBLIC
                           "${RenderTests_BINARY_DIR}/src")
target_link_libraries(wayland-protocol-xdg_shell PUBLIC
                      wayland-protocol-base
                      wayland-protocol-core)

add_custom_command(
     OUTPUT "xdg_decoration_unstable_v1.cc"
            "xdg_decoration_unstable_v1.h"
     COMMAND wayland-protocol-generator
     ARGS    "${RenderTests_SOURCE_DIR}/third_party/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml"
             "${RenderTests_SOURCE_DIR}/third_party/wayland-protocols/stable/xdg-shell/xdg-shell.xml"
             "${RenderTests_SOURCE_DIR}/third_party/wayland/protocol/wayland.xml"
     DEPENDS wayland-protocol-generator
)

add_library(wayland-protocol-xdg_decoration_unstable_v1 xdg_decoration_unstable_v1.cc xdg_decoration_unstable_v1.h)
set_target_properties(wayland-protocol-xdg_decoration_unstable_v1 PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON)
target_include_directories(wayland-protocol-xdg_decoration_unstable_v1 PUBLIC
                           "${RenderTests_BINARY_DIR}/src")
target_link_libraries(wayland-protocol-xdg_decoration_unstable_v1 PUBLIC
                      wayland-protocol-base
                      wayland-protocol-core
                      wayland-protocol-xdg_shell)

list(APPEND LINK_LIBRARIES
     wayland-protocol-base
     wayland-protocol-core
     wayland-protocol-xdg_shell
     wayland-protocol-xdg_decoration_unstable_v1)
list(APPEND SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/core.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/core.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/egl.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/egl.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/util.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/util.h"
)

set(LINK_LIBRARIES ${LINK_LIBRARIES} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
