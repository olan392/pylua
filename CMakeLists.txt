cmake_minimum_required(VERSION 3.18)

project(pylua LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LUA REQUIRED lua)

pybind11_add_module(pylua
    src/LuaRuntime.cpp
    src/bindings.cpp
)

target_include_directories(pylua PRIVATE
    ${LUA_INCLUDE_DIRS}
    include
)

target_link_libraries(pylua PRIVATE
    ${LUA_LIBRARIES}
)

target_compile_options(pylua PRIVATE
    ${LUA_CFLAGS_OTHER}
)
