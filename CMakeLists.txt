cmake_minimum_required(VERSION 3.20)

project( cplusplus_tutorial )

include("cmakeconf/compiler_conf.cmake")
include("cmakeconf/building_output.cmake")

add_subdirectory( samples )
