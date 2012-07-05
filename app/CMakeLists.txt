cmake_minimum_required(VERSION 2.6)

set(SRCS
    main.cpp
    )

include_directories("${PROJECT_SOURCE_DIR}/generator/include")

add_executable(example ${SRCS})
target_link_libraries(example pthread)
