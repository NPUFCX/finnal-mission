cmake_minimum_required(VERSION 3.10)
project(final_mission)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR /home/fcx/openCV/opencv-4.0.1/build)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(final_mission main.cpp)
target_link_libraries(final_mission ${OpenCV_LIBS})
target_link_libraries(final_mission Threads::Threads)