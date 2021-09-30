set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable( perfect_forwarding)

target_sources( perfect_forwarding
PRIVATE
    ${src}
)