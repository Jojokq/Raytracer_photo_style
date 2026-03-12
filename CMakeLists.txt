cmake_minimum_required(VERSION 4.2.1)
project(RAYTRACING LANGUAGES CXX)

set ( EXTERNAL
  src/external/stb_image.h
  src/external/stb_image_write.h
)

set (IMG_GEN_1
    src/first_iter/main.cc
    src/first_iter/camera.h
    src/first_iter/color.h
    src/first_iter/common_header.h
    src/first_iter/hittable_list.h
    src/first_iter/hittable.h
    src/first_iter/interval.h
    src/first_iter/material.h
    src/first_iter/ray.h
    src/first_iter/sphere.h
    src/first_iter/vec3.h
    )

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # /wd #### - Disable warning
    # /we #### - treat warning as error
    add_compile_options("/W4")      # Enable level-4 warnings
    add_compile_options("/we 4265") # Class has virtual functions, but its non-trivial destructor is not virtual
    add_compile_options("/we 5204") # Class has virtual functions, but its trivial destructor is not virtual
    add_compile_options("/wd 4100") # unreferenced formal parameter
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wmaybe-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wsometimes-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
endif()

#Executables
add_executable (First_iteration_of_ray_tracer src/first_iter/main.cc)
add_executable(Second_iteration_of_ray_tracer src/second_iter/main.cc)