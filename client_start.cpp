#include <iostream>
#include "Client/Client.h"
#include <thread>

int main()
{
	std::cout << "Put name: ";
	std::string name, server_IP;
	getline(std::cin, name);
	std::cout << std::endl;
	std::cout << "Put server IP: ";
	getline(std::cin, server_IP);

	Client client(server_IP, 8080);
	if (!client.Connect())
		return 1;

	std::thread receiverThread(&Client::receiveFromServer, &client);
	receiverThread.detach();

	std::string request(1024, 0);
	client.sendToServer(name);

	while (true)
	{
		request.clear();
		std::cout << name << ": ";

		getline(std::cin, request);

		client.sendToServer(request);
		std::cout << std::endl;
	}

	client.Disconnect();
	return 0;
}
