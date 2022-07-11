# CLionでの開発用のCMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(webserv C CXX)

find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS "srcs/**/*.cpp")
file(GLOB INCS "srcs/**")

file(GLOB TESTS "tests/unit_test/*.cpp")

include_directories(${INCS})

add_library(webserv
        STATIC
        ${SRCS}
)

add_executable(main
        ${TESTS}
)

target_link_libraries(main
        PRIVATE
        GTest::GTest
        GTest::Main
        webserv
)
