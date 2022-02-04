cmake_minimum_required(VERSION 3.21)
project(rec1 C)

add_executable(talk talk.c)
target_compile_features(talk PRIVATE c_std_17)
target_compile_options(talk PRIVATE -Wall -Wextra -Wpedantic -Werror
    -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable)
