cmake_minimum_required(VERSION 3.20)
project(wzq2)

set(CMAKE_CXX_STANDARD 14)
set(EGE_INC_DIR C:/EGE20.08/include)
set(EGE_LINK_DIR C:/EGE20.08/lib/mingw64)
include_directories(${EGE_INC_DIR})
link_directories(${EGE_LINK_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(wzq2 main.cpp)
target_link_libraries(wzq2 libgraphics64.a
        libgdi32.a libimm32.a libmsimg32.a libole32.a
        liboleaut32.a libwinmm.a libuuid.a libgdiplus.a)
