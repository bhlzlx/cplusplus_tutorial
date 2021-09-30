set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.h
)

add_library (base64_dynamic SHARED)

target_compile_definitions( base64_dynamic
PRIVATE
    -DBASE64_SHARED
)

target_sources( base64_dynamic
PRIVATE
    ${src}
)