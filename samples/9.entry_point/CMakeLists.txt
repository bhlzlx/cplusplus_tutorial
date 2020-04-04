project( entry_point )

set( src 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

if(MINGW)
    set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -e YetAnotherEntry" )
endif()

if(MSVC)
    set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  /ENTRY:\"YetAnotherEntry\" " )
endif()

add_executable( entry_point
    ${src}
)