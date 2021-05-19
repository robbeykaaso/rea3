cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

#if (CMAKE_BUILD_TYPE STREQUAL Debug)
#set(${OUT} )
if (NOT DEFINED MS)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/lib/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/lib/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/bin/${CMAKE_BUILD_TYPE})
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/install/bin)
endif()

project(REA)

#message(":${CMAKE_BUILD_TYPE}")

add_definitions(-DEXPORTDLL)

find_package(Qt5 COMPONENTS Core Widgets Quick REQUIRED)

# 根据库文件代码生成动态库
add_library(REA SHARED
    "qml.qrc"
    "include/util.h"
    "include/rea.h"
    "include/reaQML.h"
    "include/reaJS.h"
    "include/tess.h"
    "include/qsgModel.h"
    "include/qsgBoard.h"
    "include/command.h"
    "include/storage.h"

    "src/util.cpp"
    "src/rea.cpp"
    "src/reaQML.cpp"
    "src/reaJS.cpp"
    "src/dllloader.cpp"
    "src/qsgModel.cpp"
    "src/qsgBoard.cpp"
    "src/command.cpp"
    "src/storage.cpp"
    )

# 创建动态库别名

#add_library(hello::library ALIAS REA)

target_link_libraries(REA PRIVATE Qt5::Core Qt5::Widgets Qt5::Quick)
# 包含指定头文件所在的目录
target_include_directories(REA PUBLIC  ${PROJECT_SOURCE_DIR}/include)
#target_include_directories(REA PUBLIC  ${PROJECT_SOURCE_DIR}/imageboardplugins)

# 创建可执行程序

#add_executable(hello_binary
#src/main.cpp
#)

# 链接动态库文件
#target_link_libraries( hello_binary PRIVATE hello::library)

#if (NOT DEFINED MS)
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY2 ${CMAKE_CURRENT_LIST_DIR}/../build-app-Desktop_Qt_5_13_2_MSVC2017_64bit-${CMAKE_BUILD_TYPE}/plugin)
#else()
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY2 ${MS})
#endif()

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    "${CMAKE_SOURCE_DIR}/qml"
#    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY2}/../qml"
#    COMMENT "copying resource.")

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    "${CMAKE_SOURCE_DIR}/html"
#    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY2}/../html"
#    COMMENT "copying resource.")
