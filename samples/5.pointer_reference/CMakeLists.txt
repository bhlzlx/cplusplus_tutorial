set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_executable( ptrref)

target_sources(ptrref
PRIVATE
    ${src}
)