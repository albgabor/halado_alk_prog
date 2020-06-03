cmake_minimum_required(VERSION 3.7)
project(bead_gpu VERSION 0.1.0)

find_package(OpenCL REQUIRED)

include(CTest)
enable_testing()

set(Files_KRNS pipe.cl)
get_filename_component(Folder_KRNS
   ${Files_KRNS}
   ABSOLUTE)


add_executable(bead_gpu main.cpp scatter.hh cl2.hpp ${Files_KRNS})

set_target_properties(${PROJECT_NAME}
   PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
)
target_link_libraries(${PROJECT_NAME}
   PRIVATE
      OpenCL::OpenCL
)

target_compile_definitions(${PROJECT_NAME}
   PRIVATE
      CL_TARGET_OPENCL_VERSION=120
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
      -Wall     # Turn on all warnings
      -Wextra   # Turn on even more warnings
      -pedantic # Turn on strict language conformance
      -Wno-ignored-attributes # Console blows up due to cl.h
    >
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4          # Turn on all (sensible) warnings
      /permissive- # Turn on strict language conformance
      /EHsc        # Specify exception handling model
    >
)

source_group("Kernels" FILES ${Files_KRNS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)