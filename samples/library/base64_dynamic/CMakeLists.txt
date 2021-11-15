add_library(base64_dynamic SHARED)
target_sources( base64_dynamic
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
)