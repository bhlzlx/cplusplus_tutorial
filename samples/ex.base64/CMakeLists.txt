set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.h
)

add_library (base64 STATIC)

target_sources(  base64 PRIVATE
${src}
)