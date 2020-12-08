# ============= dynamic base64 implementation =============

project( base64_dynamic )
add_library ( base64_dynamic SHARED )
target_include_directories( base64_dynamic
    PUBLIC ./ex.base64_dynamic
)
target_sources( base64_dynamic
    PRIVATE ./ex.base64_dynamic/base64.cpp
)
target_compile_options( base64_dynamic 
    PUBLIC ${CMAKE_CXX_COMPILE_FLAGS}
    PRIVATE /D BASE64_SHARED
)

# ============= static base64 implementation =============

project( base64 )
add_library ( base64 STATIC )
target_sources( base64
    PRIVATE ./ex.base64/base64.cpp
)
target_include_directories( base64
    PUBLIC ./ex.base64
)
project( libs )

# ============= demo projects =============

add_subdirectory( 0.helloworld )
add_subdirectory( 1.basics )
add_subdirectory( 4.memory_management )
add_subdirectory( 2.forward_declare )
add_subdirectory( 3.linkage )
add_subdirectory( 5.pointer_reference )
add_subdirectory( 6.perfect_forwarding )
add_subdirectory( 7.multi_threading )
add_subdirectory( 8.pointer_cast )
add_subdirectory( 9.entry_point )
add_subdirectory( 10.polymorphism )
add_subdirectory( 11.multiInher_polymorph )
add_subdirectory( 12.member_offset )
add_subdirectory( 13.lrucache )
add_subdirectory( 14.BigInteger )