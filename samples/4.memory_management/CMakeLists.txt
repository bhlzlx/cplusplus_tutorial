set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable( memory_management)

target_sources( memory_management
PRIVATE
    ${src}
)