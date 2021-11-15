add_library(base64 STATIC)
target_sources( base64 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
)

target_include_directories( base64
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)