cmake_minimum_required(VERSION 3.2)
project(pyplay)

# This line includes our main() funtion, which is a test
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")

include_directories(/opt/local/Library/Frameworks/Python.framework/Versions/3.4/include/python3.4m/)
link_libraries(/opt/local/Library/Frameworks/Python.framework/Versions/3.4/lib/python3.4/config-3.4m/libpython3.4.dylib)

add_executable(pyplay spammodule.c)
add_executable(bigram bigram.c)