cmake_minimum_required(VERSION 3.0)
project(webserv C CXX)

find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS "../srcs/**/*.c")

include_directories("../libft/includes")
include_directories("../includes")

add_library(webserv
        STATIC
#        ${SRCS}
#        ../main.c
)

add_executable(main
        main.cpp
        utils.cpp
        )

target_link_libraries(main
    PRIVATE
    GTest::GTest
    GTest::Main
    webserv
)
