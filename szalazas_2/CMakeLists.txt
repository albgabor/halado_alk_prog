cmake_minimum_required(VERSION 3.0.0)
project(szalazas_2 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(szalazas_2 main_parallel_game_of_life.cpp)
#add_executable(szalazas_2 main_parallel_line_fitting.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
