#pragma once
#include "ClientHandler.h"

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>

#include <WS2tcpip.h>
class ClientHandler;
class Server
{
private:
	int m_port;
	char *m_serverIP;
	SOCKET m_listenSocket;
	// std::vector<SOCKET> m_clientSockets;
	std::vector<ClientHandler *> m_clientHandlers;

public:
	bool m_isRunning;
	Server(int port, char *server_IP = nullptr);
	~Server();
	void start(); // Start listening for incoming connections
	void stop();  //  Stop the server and close all client connections

	int getPort() const;
	int getClientCount() const;

	//  Send a message to all connected clients
	void broadcastMessage(const char *message, const char *sender = nullptr);

	// Send a message to a specific client
	void sendMessage(const char *message, const char *recipient, const char *sender = nullptr);

private:
	// Accept incoming client connections
	// Accepts incoming client connections and creates a new `ClientHandler`
	// instance for each new connection.
	void acceptConnections();

	// Remove a client handler from the list of active handlers
	void removeClientHandler(ClientHandler *clientHandler);
};
