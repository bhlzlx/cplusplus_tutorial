# 使用 CMake 组织 QT5 工程

## CMake 代码示例

```cmake
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
```

## CMAKE_FRAMEWORK_PATH

CMAKE_FRAMEWORK_PATH 跟QT没关系，但是我们找QT库的时候需要这个路径去查找QT的库，所以我们提前把这个路径设置好，注意不同的编译器有不同的路径

## find_package(Qt5Core REQUIRED)

刚才设置的目录里有 名为 **Qt5Core** 的文件夹，内含 ${name}-config.cmake，里面包含头文件和静态库的信息，供编译器编译链接使用

## 自动处理Qt5的资源

```
set_target_properties(HelloQT5 PROPERTIES AUTOMOC ON)
set_target_properties(HelloQT5 PROPERTIES AUTOUIC ON)
```

Qt5的源文件和界面文件需要使用预处理工具预处理一下才能编译，CMake自带了这么一个自动处理的功能，这里我们把这个功能打开