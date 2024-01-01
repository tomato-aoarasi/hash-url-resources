# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.16)

# set(VCPKG_ROOT "/opt/vcpkg")
# set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
# CACHE STRING "Vcpkg toolchain file")
link_directories(/usr/lib)
link_directories(/usr/local/lib)

set(LOCAL_INCLUDE /usr/local/include)
set(GLOBAL_INCLUDE /usr/include)

include_directories(LOCAL_INCLUDE)
include_directories(GLOBAL_INCLUDE)

project ("hash-url-resources")

# 设置项目根节点
set(PROJECT_ROOT_NODE ${CMAKE_CURRENT_SOURCE_DIR})


# 输出项目信息
message(STATUS ${PROJECT_NAME} ${PROJECT_VERSION_MAJOR} ${PROJECT_VERSION_MINOR})

# 使用C++20
set(CATKIN_TOPLEVEL TRUE)
set(CMAKE_CXX_STANDARD 20)

# 三方库导入相关
# find_package(Boost REQUIRED COMPONENTS system)
find_library(Jsoncpp_LIBRARY jsoncpp)

if(NOT Jsoncpp_LIBRARY)
    message(FATAL_ERROR "jsoncpp library not found")
endif()

find_library(Trantor_LIBRARY trantor)

if(NOT Trantor_LIBRARY)
    message(FATAL_ERROR "Trantor library not found")
endif()

# find_library(Cryptopp_LIBRARY cryptopp)

# if(NOT Cryptopp_LIBRARY)
#     message(FATAL_ERROR "cryptopp library not found")
# endif()

find_package(PkgConfig REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(spdlog REQUIRED)
find_package(Drogon REQUIRED)
# 三方库导入相关结束

# 将指定目录文件所有复制到二进制文件目录
file(GLOB allCopyFiles  "${PROJECT_ROOT_NODE}/resources/*")
# file(GLOB_RECURSE allCopyFiles  "${PROJECT_ROOT_NODE}/resource/*")
file(COPY ${allCopyFiles} DESTINATION ${PROJECT_ROOT_NODE}/bin)

# 指定最终的可执行文件的位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_NODE}/bin)

# 以当前目录为基准
include_directories(${PROJECT_ROOT_NODE}/src)
# 将源代码添加到此项目的可执行文件。
# aux_source_directory(${PROJECT_ROOT_NODE}/src SRC_LIST)
set(MAIN ${PROJECT_ROOT_NODE}/src/main.cpp)
add_executable (${PROJECT_NAME} ${MAIN})

set(sources)

file(GLOB_RECURSE sources_glob "${PROJECT_ROOT_NODE}/src/*")

foreach (source ${sources_glob})
    string(REPLACE "${SOURCE_DIR}/" "" RELATIVE_PATH ${source})
    if (NOT(${RELATIVE_PATH} STREQUAL "main.cpp"))
        list(APPEND sources "${source}")
    endif ()

    unset(RELATIVE_PATH)
endforeach ()

target_sources(${PROJECT_NAME} PUBLIC ${sources})

unset(SOURCE_DIR)
unset(sources_glob)
unset(sources)

# 动态库链接
target_link_libraries(${PROJECT_NAME} PRIVATE ${Trantor_LIBRARY} ${Jsoncpp_LIBRARY} drogon jsoncpp yaml-cpp spdlog::spdlog)

message(G:${CMAKE_C_COMPILER_VERSION},GXX:${CMAKE_CXX_COMPILER_VERSION},CXX STANDARD:${CMAKE_CXX_STANDARD})
message(final)