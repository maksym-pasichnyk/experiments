cmake_minimum_required(VERSION 3.27)
project(stl)

add_library(stl STATIC src/stl/Arc.hpp src/stl/Box.hpp src/stl/Debug.hpp src/stl/Defer.hpp src/stl/defines.hpp src/stl/Enum.hpp src/stl/Lazy.hpp src/stl/lib.cpp src/stl/lib.hpp src/stl/Option.hpp src/stl/Overloads.hpp src/stl/pch.hpp src/stl/Result.hpp src/stl/Trait.hpp)
set_target_properties(stl PROPERTIES CXX_STANDARD 26 CXX_EXTENSIONS off)
target_include_directories(stl PUBLIC src)
