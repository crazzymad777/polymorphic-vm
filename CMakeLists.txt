cmake_minimum_required(VERSION 2.8.9)
project (polymorphic-vm)

include_directories(includes/)
add_executable(povm src/main.c src/povm.c)
target_link_libraries(povm m)

