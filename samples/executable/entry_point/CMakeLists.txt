add_executable(entry_point)
target_sources(entry_point 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
)

if(MINGW)
    target_link_options( entry_point 
        PRIVATE "${CMAKE_EXE_LINKER_FLAGS} -e YetAnotherEntry"
    )
endif()

if(MSVC)
    set( ENTRY_NAME "\"YetAnotherEntry\"")
    target_link_options( entry_point 
        PRIVATE "${CMAKE_EXE_LINKER_FLAGS}  /ENTRY:${ENTRY_NAME}"
    )
endif()