project( HelloQT5 )

# set_source_files_properties(main.cpp PROPERTIES SKIP_AUTOMOC ON)

set( CMAKE_FRAMEWORK_PATH ${CMAKE_FRAMEWORK_PATH} "E:/IDE/Qt/Qt5.10.1/5.10.1/msvc2015_64/lib/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

set( HelloQT5_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloQT5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloQT5.h
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloQT5.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloQT5.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable( HelloQT5 WIN32 
    ${HelloQT5_SOURCE}
)

target_link_libraries(
    HelloQT5
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)

set_target_properties(HelloQT5 PROPERTIES AUTOMOC ON)
set_target_properties(HelloQT5 PROPERTIES AUTOUIC ON)