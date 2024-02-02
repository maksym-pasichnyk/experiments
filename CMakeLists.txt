cmake_minimum_required(VERSION 3.27)
project(experiments)
include(FetchContent)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

FetchContent_Declare(stl SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/stl )
FetchContent_MakeAvailable(stl)

add_executable(experiments src/main.cpp src/vkh.hpp src/gpu/allocator.hpp src/gpu/vtable.hpp src/gpu/core.cpp src/gpu/core.hpp src/gpu/lib.hpp src/FreeList.hpp
)
set_target_properties(experiments PROPERTIES CXX_STANDARD 26 CXX_EXTENSIONS off)
target_link_libraries(experiments PUBLIC stl)
target_link_libraries(experiments PUBLIC SDL2::SDL2)
target_link_libraries(experiments PUBLIC Vulkan::Vulkan)
target_compile_definitions(experiments PUBLIC -DVK_NO_PROTOTYPES)
target_compile_definitions(experiments PUBLIC -DVK_ENABLE_BETA_EXTENSIONS)
