project( base64_dynamic )

ADD_DEFINITIONS(-DBASE64_SHARED)


set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.h
)

add_library ( base64_dynamic SHARED
    ${src}
)