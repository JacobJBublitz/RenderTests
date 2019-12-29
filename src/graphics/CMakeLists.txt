list(APPEND SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/window.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/window.h"
)

add_subdirectory(egl)
add_subdirectory(wayland)

set(LINK_LIBRARIES ${LINK_LIBRARIES} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
