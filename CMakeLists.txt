cmake_minimum_required(VERSION 3.25.1)
project(SNAP)
add_executable(SNAP commandline.cpp)
install(TARGETS SNAP DESTINATION bin)