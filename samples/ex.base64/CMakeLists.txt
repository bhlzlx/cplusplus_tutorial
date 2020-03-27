project( base64 )

set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.h
)

add_library ( base64 STATIC
    ${src}
)