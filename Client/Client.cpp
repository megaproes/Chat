#include "Client.h"

Client::Client(const std::string &serverIP, int port)
    : m_socket(INVALID_SOCKET), m_serverAddr(nullptr)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return;
	}
	addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(serverIP.c_str(), std::to_string(port).c_str(), &hints, &m_serverAddr);
	if (iResult != 0)
	{
		std::cerr << "getaddrinfo failed" << std::endl;
		WSACleanup();
		return;
	}

	m_socket = socket(m_serverAddr->ai_family, m_serverAddr->ai_socktype, m_serverAddr->ai_protocol);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(m_serverAddr);
		WSACleanup();
		return;
	}
}

bool Client::Connect()
{
	int iResult = connect(m_socket, m_serverAddr->ai_addr, (int)m_serverAddr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(m_serverAddr);
		closesocket(m_socket);
		WSACleanup();
		return 0;
	}
	std::cout << "Connected to server." << std::endl;
	return 1;
}

void Client::sendToServer(std::string message)
{
	int iResult = send(m_socket, message.c_str(), message.length(), 0);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
	}
}

void Client::receiveFromServer()
{
	while (true)
	{

		char buffer[1024] = {};
		int iResult = recv(m_socket, buffer, 1024, 0);

		if (iResult < 0)
		{
			// ошибка при чтении из сокета
		}
		else if (iResult == 0)
		{
			// соединение закрыто
			std::cout << "Connection closed . . ." << std::endl;
		}
		else
		{
			// успешно получены данные, обработка
			std::cout << std::endl
					<< buffer << std::endl;
		}
	}
}

Client::~Client() {}
