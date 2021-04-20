cmake_minimum_required(VERSION 2.6)

project(webd CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "debug")
endif()

message(STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message(STATUS "PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH})
message(STATUS "CMAKE_INCLUDE_PATH: " ${CMAKE_INCLUDE_PATH})

if(NOT NETWORKER_PATH)
    set(NETWORKER_PATH "/usr")
    message(STATUS "NETWORKER_PATH: " ${NETWORKER_PATH})
endif()

if(NOT NETWORKER_LIB_PATH)
    set(NETWORKER_LIB_PATH ${NETWORKER_PATH} "/local")
    message(STATUS "NETWORKER_LIB_PATH: " ${NETWORKER_LIB_PATH})
endif()

set(CXX_FLAGS
    -g
    -Wall
    -std=c++17
    -Werror
    -rdynamic
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# find_path 用来在指定路径中搜索文件名
find_path(NetWorker_INCLUDE_DIR networker "${NETWORKER_PATH}/include")
find_path(NetWorker_LIBRARY_DIR NAMES "libnetworker_net.a" "${NETWORKER_LIB_PATH}/lib" PATH_SUFFIXES lib)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NetWorker_LIBRARY_DIR})
message(STATUS "NetWorker_INCLUDE_DIR: " ${NetWorker_INCLUDE_DIR})
message(STATUS "NetWorker_LIBRARY_DIR: " ${NetWorker_LIBRARY_DIR})

# include_directories 将指定目录添加到编译器头文件搜索路径之下，指定的目录被解释成当前源码路径的相对路径
include_directories(${NetWorker_LIBRARY_DIR})

# find_library 该命令用于搜索动态文件路径，里面的内容为自定义的变量名、动态文件名、具体路径
find_library(networker_base networker_base)
find_library(networker_net networker_net)
message(STATUS ${networker_base})
message(STATUS ${networker_net})

include_directories(${PROJECT_SOURCE_DIR})

set(src_dir
    src/HttpContext.cpp
    src/HttpResponse.cpp
    src/HttpResponse.cpp
    src/HttpServer.cpp
    src/Entry.cpp
    src/TemplateReplace.cpp
)

add_executable(webd ${src_dir} src/main.cpp)

# target_link_libraries 这个指令可以用来添加需要链接的共享库
target_link_libraries(webd ${networker_net})
target_link_libraries(webd ${networker_base})
target_link_libraries(webd pthread rt)