set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(member_offset)

target_sources(member_offset
PRIVATE
    ${src}
)