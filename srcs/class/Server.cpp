#include "../header/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
	{
		std::cerr << "Error while creating socket." << std::endl;
		throw std::exception();
	}

	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);

	if (bind(_serverSocket, reinterpret_cast<struct sockaddr *>(&_serverAddress), sizeof(_serverAddress)) == -1)
	{
		close(_serverSocket);
		std::cerr << "Erreur when binding socket to server address." << std::endl;
		throw std::exception();
	}

	if (listen(_serverSocket, 5) == -1)
	{
		close(_serverSocket);
		std::cerr << "Erreur when binding socket to server address." << std::endl;
		throw std::exception();
	}

	pollfd server = {_serverSocket, POLLIN, 0};
	_clientSockets.push_back(server);
}

void Server::start()
{
	std::cout << "IRC Server awaiting on port " << _port << "." << std::endl;

	while (42)
	{
		if (poll(_clientSockets.data(), _clientSockets.size(), -1) <= 0)
		{
			std::cerr << "Error on poll function." << std::endl;
			throw std::exception();
		}
//		std::cout << "POLL " << poll(_clientSockets.data(), _clientSockets.size(), -1) << std::endl;

		for (size_t i = 0; i < _clientSockets.size(); ++i)
		{
			if (_clientSockets[i].fd == _serverSocket && _clientSockets[i].revents & POLLIN)
				newConnexion();
			else if (_clientSockets[i].revents & POLLIN)
				newMessage((int)i);
		}
	}
}

void Server::newConnexion()
{
	std::cout << "CONNEXION" << std::endl;
	sockaddr_in clientAddress = {};
	socklen_t clientAddressLength = sizeof(clientAddress);

	int clientSocket = accept(_serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddress),
							  &clientAddressLength);
	if (clientSocket < 0)
	{
		std::cerr << "Error on new connexion." << std::endl;
		throw std::exception();
	}

	pollfd newClient = {clientSocket, POLLIN, 0};
	_clientSockets.push_back(newClient);

	std::cout << "New connexion. Socket : " << clientSocket << std::endl;
}

void Server::newMessage(int clientNumber)
{
	std::cout << "MSG" << std::endl;
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	size_t bytesRead = recv(_clientSockets[clientNumber].fd, buffer, sizeof(buffer) - 1, 0);
	if (bytesRead <= 0)
		disconnexion(clientNumber);
	else
		handleMessage(buffer, clientNumber);
}

void Server::disconnexion(int clientNumber)
{
	close(_clientSockets[clientNumber].fd);
	_clientSockets.erase(_clientSockets.begin() + clientNumber);

	std::cout << "Disconnected client. Socket : " << _clientSockets[clientNumber].fd << std::endl;
}

void Server::handleMessage(char *buffer, int clientNumber)
{
	std::string receivedData(buffer);
	std::cout << receivedData;
	// To send message to client :
	// send(_clientSockets[clientNumber].fd, response.c_str(), response.size(), 0);
}
