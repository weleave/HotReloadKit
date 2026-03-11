cmake_minimum_required(VERSION 3.28)
project(hot_reload_tool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(hot_reload_tool
        src/types.cpp
        src/file_watcher.cpp
        src/hot_reload_app.cpp
        src/main.cpp

        include/hotreload/directory_scanner.h
        include/hotreload/types.h
        include/hotreload/resource_store.h
        include/hotreload/file_watcher.h
        include/hotreload/hot_reload_app.h
        src/directory_scanner.cpp
        src/resource_store.cpp
)

target_include_directories(hot_reload_tool PRIVATE include)

if (MSVC)
    target_compile_options(hot_reload_tool PRIVATE /W4 /permissive-)
else()
    target_compile_options(hot_reload_tool PRIVATE -Wall -Wextra -Wpedantic)
endif()