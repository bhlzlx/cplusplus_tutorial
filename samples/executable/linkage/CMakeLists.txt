# target_link_libraries(  linkage
#     PUBLIC base64
# )

target_sources( linkage
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib.cpp
)