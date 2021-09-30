set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/samples/ex.base64
)

add_executable( linkage
    ${src}
)

link_directories (

)

target_link_libraries( 
    linkage
    base64
)