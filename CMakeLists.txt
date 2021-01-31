cmake_minimum_required(VERSION 3.2)

project( cplusplus_tutorial )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmakeModules )

include( basicEnv )
include( outputPath )

add_subdirectory( samples )