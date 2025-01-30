cmake_minimum_required(VERSION 3.28)
project(tcp_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0A00 -std=c++20 -lws2_32 -lmswsock")

add_executable(tcp_server main.cpp
        src/WebServer.cpp
        include/WebServer.h)

target_compile_definitions(tcp_server PRIVATE _WIN32_WINNT=0x0A00)

# Link Windows networking libraries for ASIO
target_link_libraries(tcp_server PRIVATE ws2_32 mswsock)

