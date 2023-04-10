#include "Server.h"
#include <sstream>
Server::Server(int port, char *server_IP)
    : m_port(port), m_serverIP(server_IP) {}

void Server::start()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return;
	}
	addrinfo hints = {}, *result = nullptr;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(nullptr, std::to_string(m_port).c_str(), &hints, &result);
	if (iResult != 0)
	{
		std::cerr << "getaddrinfo failed" << std::endl;
		WSACleanup();
		return;
	}

	m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET)
	{
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult)
	{
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(m_listenSocket);
		WSACleanup();
		return;
	}
	freeaddrinfo(result);

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
		closesocket(m_listenSocket);
		WSACleanup();
		return;
	}
	std::cout << "Server started on port: " << m_port << std::endl;
	m_isRunning = true;

	std::cout << "Waiting for the clients . . ." << std::endl;
	std::thread acceptThread{&Server::acceptConnections, this};
	acceptThread.join();
}

void Server::acceptConnections()
{
	while (m_isRunning)
	{
		SOCKET clientSocket = accept(m_listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
			continue;
		}

		ClientHandler *clientHandler = new ClientHandler(clientSocket, this);
		m_clientHandlers.push_back(clientHandler);

		std::thread handlerThread(&ClientHandler::handle, clientHandler);
		handlerThread.detach();
		std::cout << "Client " << clientSocket << " connected" << std::endl;
	}
}

void Server::broadcastMessage(const char *message, const char *sender)
{
	for (auto &clientHandler : m_clientHandlers)
	{
		if (/*sender == nullptr ||*/ clientHandler->getName() != sender)
		{
			std::string mes = std::string(sender) + ':' + ' ' + std::string(message);

			clientHandler->sendMessage(mes.c_str());
		}
	}
}

Server::~Server() {}