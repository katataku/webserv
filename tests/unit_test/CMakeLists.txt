cmake_minimum_required(VERSION 3.0)
project(webserv C CXX)

find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS "../../srcs/**/*.cpp")
file(GLOB INCS "../../srcs/**")

include_directories(${INCS})

add_library(webserv
        STATIC
        ${SRCS}
)

add_executable(main
        main.cpp
        utils.cpp
        HTTP.cpp
        ConfigParser.cpp
        CreateServerLocation.cpp
        Transaction.cpp
        CGI.cpp
)

target_link_libraries(main
        PRIVATE
        GTest::GTest
        GTest::Main
        webserv
)
