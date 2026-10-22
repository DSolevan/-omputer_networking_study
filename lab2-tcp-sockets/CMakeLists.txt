add_executable(lab2_server server.cpp)
add_executable(lab2_client client.cpp)

target_link_libraries(lab2_server PRIVATE ws2_32)
target_link_libraries(lab2_client PRIVATE ws2_32)

target_link_options(lab2_server PRIVATE -static-libgcc -static-libstdc++)
target_link_options(lab2_client PRIVATE -static-libgcc -static-libstdc++)