list(APPEND SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/context.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/context.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/display.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/display.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/internal.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/internal.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/surface.cc"
     "${CMAKE_CURRENT_SOURCE_DIR}/surface.h"
)

set(SOURCES ${SOURCES} PARENT_SCOPE)
