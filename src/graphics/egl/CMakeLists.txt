list(APPEND SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/display.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/display.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/internal.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/internal.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/native_display.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/native_window.h"
)

set(SOURCES ${SOURCES} PARENT_SCOPE)
