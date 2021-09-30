set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable( pointer_cast)

target_sources( pointer_cast
PRIVATE
    ${src}
)