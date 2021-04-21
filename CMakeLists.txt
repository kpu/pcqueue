cmake_minimum_required(VERSION 3.1)

project(pcqueue)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

add_executable(test util/test.cc)
target_link_libraries(test PUBLIC Threads::Threads)
