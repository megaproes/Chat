#pragma once
#include "Server.h"

#include <iostream>
#include <string>
#include <vector>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

// The `ClientHandler` class is responsible for handling communication
// with a single connected client.
// It receives messages from the client, processes them,
// and sends responses back to the client.
class Server;
class ClientHandler
{
private:
	SOCKET m_socket;
	std::string m_name;
	Server *m_server;
	std::string m_message;

public:
	ClientHandler(SOCKET socket, Server *server);

	// The main method that handles communication with the client.
	// It receives messages from the client using `receiveMessage()` and processes them.
	void handle();					    //  Handle communication with the client
	void sendMessage(const char *message); //  Send a message to the client
	void setName(const std::string &name); // Set the client's name
	std::string getName() const;		    // Get the client's name

private:
	int receiveMessage(); // Receive a message from a client
	void receiveNameMessage();
};
