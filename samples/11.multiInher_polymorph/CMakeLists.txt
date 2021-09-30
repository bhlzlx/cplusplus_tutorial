set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable( multiInher_polymorph)

target_sources( multiInher_polymorph
PRIVATE
    ${src}
)