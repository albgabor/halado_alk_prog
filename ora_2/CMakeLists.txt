cmake_minimum_required(VERSION 3.0.0)
project(projekt1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(projekt1 main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set_target_properties(projekt1 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)