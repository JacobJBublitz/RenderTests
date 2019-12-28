list(APPEND SOURCES 
     "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
)

add_subdirectory(graphics)
add_subdirectory(posix)
add_subdirectory(util)

set(LINK_LIBRARIES ${LINK_LIBRARIES} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
