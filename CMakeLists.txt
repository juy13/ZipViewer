cmake_minimum_required(VERSION 3.16)

project(ZipViewer
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -pthread")

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


include_directories(${CMAKE_CURRENT_LIST_DIR}/include)


find_package(minizip 3.0.2 EXACT REQUIRED PATHS ${CMAKE_CURRENT_LIST_DIR}/lib/cmake)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core)

message(${CMAKE_CURRENT_LIST_DIR})

add_executable(ZipViewer src/main.cpp
        src/mainwindow.cpp inc/mainwindow.h
        src/TableModel.cpp inc/TableModel.h
        src/FileInfo.cpp inc/FileInfo.h)

target_compile_definitions(ZipViewer PRIVATE -DHAVE_STDINT_H;-DHAVE_INTTYPES_H;-D_POSIX_C_SOURCE=200112L -DHAVE_ZLIB;-DHAVE_LZMA;-DLZMA_API_STATIC;-DHAVE_ICONV;-DMZ_ZIP_SIGNING;-DHAVE_PKCRYPT;-DHAVE_WZAES)
target_compile_definitions(ZipViewer PRIVATE -DHAVE_COMPAT)

target_include_directories(ZipViewer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(ZipViewer  MINIZIP::minizip)
target_link_libraries(ZipViewer  Qt5::Widgets)
target_link_libraries(ZipViewer  Qt5::Core)