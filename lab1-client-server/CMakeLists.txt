add_executable(lab1_server server.cpp)
add_executable(lab1_client client.cpp)

target_link_options(lab1_server PRIVATE -static-libgcc -static-libstdc++)
target_link_options(lab1_client PRIVATE -static-libgcc -static-libstdc++)