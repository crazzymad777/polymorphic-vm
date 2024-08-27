cmake_minimum_required(VERSION 2.8.9)
project (polymorphic-vm)

include_directories(includes/)
add_executable(povm src/main.c src/povm.c src/datum.c src/datum-casts.c src/datum_compare.c src/datum_math.c src/datum_op_equals.c src/datum-pair.c src/result.c src/povm-gen.c src/povm-info.c)
target_link_libraries(povm m)
