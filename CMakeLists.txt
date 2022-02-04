cmake_minimum_required(VERSION 3.21)
project(rec1 C)

add_executable(recitation recitation.c)
target_compile_features(recitation PRIVATE c_std_17)
target_compile_options(recitation PRIVATE -Wall -Wextra -Wpedantic -Werror
    -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable)
