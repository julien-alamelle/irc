#include "../header/Server.hpp"
#include "Commande.hpp"
#include <cstdlib>
#include "Server.hpp"

Server::Server(int port, const std::string &password) : _port(port), _password(password)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		throw SocketError();

	const int enable = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
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

User* Server::findUser(std::string user)
{
	std::map<int, User>::iterator it, end;
	end = this->_clients.end();
	for (it = this->_clients.begin(); it != end; ++it)
		if (it->second.getNickname() == user)
			return &(it->second);
	return 0;
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

		for (unsigned int i = _clientSockets.size(); i > 0; --i)
		{
			std::vector<pollfd>::iterator it = _clientSockets.begin() + i - 1;
			if (it->fd == _serverSocket && it->revents & POLLIN)
				newConnexion();
			else if (it->revents & POLLIN)
				newMessage(it);
			else if (it->revents & POLLOUT)
			{
				std::vector<std::string> &toSend = this->_clients.find(it->fd)->second.getMessages();
				for (std::vector<std::string>::iterator msg = toSend.begin(); msg != toSend.end(); ++msg)
					send(it->fd, msg->c_str(), msg->size(), 0);
				toSend.clear();
			}
			//POLLERR POLLHUP ?

			for (std::map<std::string, Channel>::iterator j = _channels.begin(); j != _channels.end() ; ++j)
				if (j->second.isEmpty())
					_channels.erase(j);
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

	pollfd newClient = {clientSocket, POLLIN | POLLOUT, 0};
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

	std::string response = "pong\n";
	std::string receivedData(buffer);
//	std::cout << CYAN << "FULL " << it->fd << ": " << receivedData << END;

	Commande cmd;
	User *user;

	size_t newLine;
	std::string line;

	if (*receivedData.rbegin() != '\n')
		receivedData += '\n';
	while ((newLine = receivedData.find('\n')) != std::string::npos)
	{
		line = receivedData.substr(0, newLine);
		if (!line.empty() && *line.rbegin() == '\r')
			line = line.substr(0, line.size() - 1);

		std::cout << CYAN << it->fd << ": " << line << END << std::endl;
		cmd.parse(line);
		response = cmd.toString();    //TODO generate the answer and sent to the right client
		std::cout << response << std::endl;

		user = &(_clients.find(it->fd)->second);
		user->addMessage(response);

		if (cmd.getCommande() == "PASS")
			cmdPass(cmd, user);
		else if (user->isPasswordOk())
		{
//			std::cout << it->fd << ": COMMAND: " << cmd.getCommande() << "|" << std::endl; //debug
			if (cmd.getCommande() == "USER")
				this->cmdUser(cmd, user);
			else if (cmd.getCommande() == "NICK")
				this->cmdNick(cmd, user);
			else if (cmd.getCommande() == "MODE")
				this->cmdMode(cmd, user);
			else if (cmd.getCommande() == "JOIN")
				this->cmdJoin(cmd, user);
			else if (cmd.getCommande() == "INVITE")
				this->cmdInvi(cmd, user);
			else if (cmd.getCommande() == "PRIVMSG")
				this->cmdPMSG(cmd, user);
			else if (cmd.getCommande() == "KICK")
				this->cmdKick(cmd, user);
			else if (cmd.getCommande() == "TOPIC")
				this->cmdTopi(cmd, user);
			else if (cmd.getCommande() == "QUIT")
				this->cmdQuit(cmd, user);
		}
		else
		{
			std::cerr << "Not logged" << std::endl;
		}

		receivedData.erase(0, newLine + 1);
	}

//	 To send message to client :
	/*if (receivedData == "ping\n")
	{
		std::cout << "pong" << std::endl;
		Server::ft_send(it->fd, response);
	}*/
}

void Server::delUserFromChannel(std::map<std::string, Channel>::iterator it, User *user)
{
	it->second.delUser(user);
	if(it->second.uBegin() == it->second.uEnd())
		this->_channels.erase(it);
	user->leaveChannel(it);
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
