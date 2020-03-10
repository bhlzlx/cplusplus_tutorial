project( forward_declare )

set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/computer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/computer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/mouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/mouse.h
)

add_executable( forward_declare
    ${src}
)