cmake_minimum_required(VERSION 2.8.9)
project (polymorphic-vm)

include_directories(includes/)
include_directories(munit/) # munit.h
add_library(povm-framework STATIC src/povm.c src/povm-stack.c src/datum.c src/datum-casts.c src/datum_compare.c src/datum_math.c src/datum_op_equals.c src/datum-pair.c src/result.c) # core part of PoVM
add_library(povm-generator-framework STATIC src/povm-gen.c src/povm-info.c) # additional part with Info and Generator
target_link_libraries(povm-generator-framework povm-framework)

add_executable(povm-test src/test/povm-test.c src/test/povm-test-call.c src/test/povm-test-add.c src/test/povm-test-bxor.c src/test/povm-test-dup.c munit/munit.c) # munit test suite
add_executable(povm src/main.c) # main executable
target_link_libraries(povm m povm-framework)
target_link_libraries(povm-test m povm-generator-framework)
