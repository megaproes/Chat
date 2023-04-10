#include "ClientHandler.h"

ClientHandler::ClientHandler(SOCKET socket, Server *server)
    : m_socket(socket), m_server(server){};

void ClientHandler::handle()
{
	// receive name
	receiveNameMessage();

	while (m_server->m_isRunning)
	{
		if (receiveMessage() == 1)
			break;

		m_server->broadcastMessage(m_message.c_str(), m_name.c_str());
	}
}

void ClientHandler::sendMessage(const char *message)
{
	int iResult = send(m_socket, message, strlen(message), 0);
}

void ClientHandler::setName(const std::string &name) { m_name = name; }
std::string ClientHandler::getName() const { return m_name; }

int ClientHandler::receiveMessage()
{
	char buffer[1024] = {};
	int iResult = recv(m_socket, buffer, sizeof(buffer), 0);

	if (iResult > 0)
	{
		m_message.clear();
		m_message = buffer;
		std::cout << buffer << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Failed to recv message" << std::endl;
		return 1;
	}
}

void ClientHandler::receiveNameMessage()
{
	char buffer[1024] = {};
	int iResult = recv(m_socket, buffer, sizeof(buffer), 0);

	if (iResult > 0)
	{
		m_name = buffer;
	}
	else
		std::cout << "Failed to recv name" << std::endl;
}
