cmake_minimum_required(VERSION 3.0.0)
project(Chat VERSION 0.1.0)


add_executable(
	Server 
	Server/Server.cpp 
	Server/Server.h 
	Server/ClientHandler.cpp 
	Server/ClientHandler.h  
	server_start.cpp)
add_executable(Client Client/Client.cpp client_start.cpp)

target_link_libraries(Server -static ws2_32)
 target_link_libraries(Client -static ws2_32)
