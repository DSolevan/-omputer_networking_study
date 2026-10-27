cmake_minimum_required(VERSION 3.10)
project(lab3_udp_sockets CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3_udp_server server.cpp)
add_executable(lab3_udp_client client.cpp)

target_link_libraries(lab3_udp_server ws2_32)
target_link_libraries(lab3_udp_client ws2_32)
