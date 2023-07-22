#include "../header/Server.hpp"
#include "Commande.hpp"

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

		for (unsigned int i = 0; i < _clientSockets.size(); ++i)
		{
			std::vector<pollfd>::iterator it = _clientSockets.begin() + i;
			if (it->fd == _serverSocket && it->revents == POLLIN)
				newConnexion();
			else if (it->revents == POLLIN)
				newMessage(it);
		}
	}
	close(_serverSocket);
}

void Server::newConnexion()
{
//	std::cout << "NEW\n";
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
	_clients.insert(std::make_pair(clientSocket, User(clientSocket)));

	std::cout << GREEN << "New connexion. Socket : " << clientSocket << END << std::endl;
}

void Server::newMessage(std::vector<pollfd>::iterator it)
{
//	std::cout << "MSG\n";
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));

	if (recv(it->fd, buffer, sizeof(buffer) - 1, 0) <= 0)
		disconnexion(it);
	else
		handleMessage(buffer, it);
}

void Server::disconnexion(std::vector<pollfd>::iterator it)
{
//	std::cout << "DISCONNEXION\n";
	int fd = it->fd;

	close(fd);
	std::cout << RED << "Disconnected client. Socket : " << _clients.find(fd)->second.getSocket() << END
			  << std::endl;

	_clients.erase(fd);
	_clientSockets.erase(it);
}

void Server::handleMessage(char *buffer, std::vector<pollfd>::iterator it)
{
//	std::cout << "HANDLE MSG\n";
	Commande cmd;

	std::string response = "pong\n";
	std::string receivedData(buffer);
	std::cout << CYAN << it->fd << ": " << receivedData << END;

	cmd.parse(receivedData);
	response = cmd.toString();	//TODO generate the answer and sent to the right client
	std::cout << response << std::endl;
	send(it->fd, response.c_str(), response.size(), 0);

//	if (receivedData == "PASS " + _password) //FIXME: do this with the parser
//		_clients.find(clientNumber)->second.setPasswordOk();

	// To send message to client :
	if (receivedData == "ping\n")
	{
		std::cout << "pong" << std::endl;
		send(it->fd, response.c_str(), response.size(), 0);
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
