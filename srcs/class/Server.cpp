#include "../header/Server.hpp"

Server::Server(int port, const std::string& password) : _port(port), _password(password)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		throw SocketError();

	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);

	if (bind(_serverSocket, reinterpret_cast<struct sockaddr *>(&_serverAddress), sizeof(_serverAddress)) == -1)
	{
		close(_serverSocket);
		throw BindError();
	}

	if (listen(_serverSocket, SOMAXCONN) == -1)
	{
		close(_serverSocket);
		throw ListenError();
	}

	pollfd server = {_serverSocket, POLLIN, 0};
	_clientSockets.push_back(server);
}

void Server::start(int &keep)
{
	std::cout << "IRC Server awaiting on port " << _port << "." << std::endl;

	while (keep)
	{
		if (poll(_clientSockets.data(), _clientSockets.size(), -1) <= 0 && keep)
		{
			close(_serverSocket);
			throw PollError();
		}

		for (size_t i = 0; i < _clientSockets.size(); ++i)
		{
			if (_clientSockets[i].fd == _serverSocket && _clientSockets[i].revents & POLLIN)
				newConnexion();
			else if (_clientSockets[i].revents & POLLIN)
				newMessage((int)i);
		}
	}
	close(_serverSocket);
}

void Server::newConnexion()
{
	sockaddr_in clientAddress = {};
	socklen_t clientAddressLength = sizeof(clientAddress);

	int clientSocket = accept(_serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddress),
							  &clientAddressLength);
	if (clientSocket < 0)
	{
		std::cerr << "Error on new connexion." << std::endl;
		throw ConnexionError();
	}

	pollfd newClient = {clientSocket, POLLIN, 0};
	_clientSockets.push_back(newClient);

	std::cout << GREEN << "New connexion. Socket : " << clientSocket << END << std::endl;
}

void Server::newMessage(int clientNumber)
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));

	if (recv(_clientSockets[clientNumber].fd, buffer, sizeof(buffer) - 1, 0) <= 0)
		disconnexion(clientNumber);
	else
		handleMessage(buffer, clientNumber);
}

void Server::disconnexion(int clientNumber)
{
	close(_clientSockets[clientNumber].fd);
	_clientSockets.erase(_clientSockets.begin() + clientNumber);

	std::cout << RED << "Disconnected client. Socket : " << _clientSockets[clientNumber].fd << END << std::endl;
}

void Server::handleMessage(char *buffer, int clientNumber)
{
	std::string response = "pong\n";
	std::string receivedData(buffer);
	std::cout << CYAN << receivedData << END;
	// To send message to client :
	if (receivedData == "ping\n")
	{
		std::cout << "pong" << std::endl;
		send(_clientSockets[clientNumber].fd, response.c_str(), response.size(), 0);
	}
}



/* EXCEPTIONS */

const char *Server::SocketError::what() const throw()
{
	return "Error while creating socket.";
}

const char *Server::BindError::what() const throw()
{
	return "Error when binding.";
}

const char *Server::ListenError::what() const throw()
{
	return "Error when listening.";
}

const char *Server::PollError::what() const throw()
{
	return "Error on poll.";
}

const char *Server::ConnexionError::what() const throw()
{
	return "Error on new connexion.";
}
