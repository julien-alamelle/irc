#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <poll.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

class Server
{
public:
	Server(int port, std::string password);
	void start();

private:
	std::vector<pollfd>	_clientSockets;
	sockaddr_in			_serverAddress;
	int					_serverSocket;
	int					_port;
	std::string			_password;

	void newConnexion();
	void newMessage(int clientNumber);
	void disconnexion(int clientNumber);
	void handleMessage(char *buffer);
};


#endif //SERVER_HPP
