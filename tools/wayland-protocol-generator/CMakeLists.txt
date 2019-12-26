add_executable(wayland-protocol-generator
               cpp_generator.cc cpp_generator.h
               generator.cc generator.h
               main.cc
               protocol.cc protocol.h)
target_link_libraries(wayland-protocol-generator PRIVATE
                      fmt
                      tinyxml2)
set_target_properties(wayland-protocol-generator PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON)
target_include_directories(wayland-protocol-generator PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR})
