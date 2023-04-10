#include <iostream>
#include "Server/Server.h"
#include <thread>
int main()
{
	Server server(8080);
	std::thread server_thread{Server::start, server};

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1000000000));
		std::cout << ". . ."
				<< "\n";
	}

	return 0;
}
