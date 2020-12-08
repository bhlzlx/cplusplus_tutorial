project( linkage )

add_executable( linkage
    ${src}
)

target_link_libraries(  linkage
    base64
)

target_sources( linkage
    PRIVATE main.cpp
    PRIVATE lib.cpp
)