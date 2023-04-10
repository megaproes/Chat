#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>

#include <WS2tcpip.h>

class Client
{
private:
	SOCKET m_socket;
	addrinfo *m_serverAddr;

public:
	Client(const std::string &serverIP, int port);
	~Client();

	bool Connect();
	void Disconnect();

	void receiveFromServer();
	
	void sendToServer(std::string message);
};