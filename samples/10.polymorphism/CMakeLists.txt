set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable( polymorphism)

target_sources( polymorphism
PRIVATE
    ${src}
)