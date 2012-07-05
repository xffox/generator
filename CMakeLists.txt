cmake_minimum_required(VERSION 2.6)

project(generator)

include(CTest)

set(WARN_FLAGS "-Wall -Wextra -Weffc++ -pedantic")
add_definitions("${WARN_FLAGS}")
add_definitions("-std=c++0x")

add_subdirectory(generator)
add_subdirectory(app)
