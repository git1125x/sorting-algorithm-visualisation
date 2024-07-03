cmake_minimum_required(VERSION 3.0)
project(raylib-test)

set(CMAKE_CXX_STANDARD 14)


add_executable(${PROJECT_NAME} src/main.cpp)
if (NOT TARGET raylib)
  find_package(raylib 2.5.0 REQUIRED)
endif()
target_link_libraries(${PROJECT_NAME} raylib)