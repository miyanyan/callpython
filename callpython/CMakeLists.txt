# CMakeList.txt: callpython 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.20)

find_package(pybind11 REQUIRED)

if(Python_FOUND)
    message(STATUS "Python found: ${Python_EXECUTABLE}")
    message(STATUS "Python version: ${Python_VERSION}")
else()
    message(FATAL_ERROR "Python not found")
endif()

# 将源代码添加到此项目的可执行文件。
add_executable (callpython "callpython.cpp" )

target_link_libraries(callpython PRIVATE pybind11::embed)