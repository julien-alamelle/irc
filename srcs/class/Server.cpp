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
		send(it->fd, response.c_str(), response.size(), 0);

		user = &(_clients.find(it->fd)->second);
		if (cmd.getCommande() == "PASS")
			cmdPass(cmd, user);
		else if (user->isPasswordOk())
		{
//			std::cout << it->fd << ": COMMAND: " << cmd.getCommande() << "|" << std::endl; //debug
			if (cmd.getCommande() == "USER")
				cmdUser(cmd, user);
			else if (cmd.getCommande() == "NICK")
				cmdNick(cmd, user);
			else if (cmd.getCommande() == "JOIN")
				cmdJoin(cmd, user);
			else if (cmd.getCommande() == "INVITE")
				cmdInvite(cmd, user);
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
		send(it->fd, response.c_str(), response.size(), 0);
	}*/
}


/* COMMANDS */

void Server::cmdPass(const Commande &cmd, User *user)
{
	if (user->isPasswordOk())
		Messages::alreadyRegistered(*user);
	if (cmd.getParams().empty())
		Messages::needMoreParams(*user, cmd);
	if (cmd.getParams()[0] == _password)
	{
		user->setPasswordOk();
		std::cout << user->getSocket() << ": " << "PASS OK\n"; //debug
	}
	else
		Messages::passwdMisMatch(*user);
}

void Server::cmdUser(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() < 4)
		Messages::needMoreParams(*user, cmd);
	else if (!user->getUsername().empty())
		Messages::alreadyRegistered(*user);
	else
	{
		std::string &realname = const_cast<std::string &>(cmd.getParams()[3]);
		for (unsigned int i = 3; i < cmd.getParams().size(); i++)
			realname += " " + cmd.getParams()[i];
		if (realname[0] != ':')
		{
			// Error: realname must start with ':'
		}
		else
		{
			user->setUsername(cmd.getParams()[0]);
			user->setRealname(realname.substr(1, realname.size() - 1));
		}
	}
}

void Server::cmdNick(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() != 1)
		Messages::needMoreParams(*user, cmd);
	else
	{
		try
		{
			user->setNickname(cmd.getParams()[0]);
			std::cout << "New NICK:" << user->getNickname() << std::endl;
		}
		catch (std::exception &e)
		{
			Messages::invalidNickName(*user);
			std::cerr << e.what() << std::endl;
			// Invalid nick
		}
	}
}

void Server::cmdJoin(const Commande &cmd, User *user)
{
	if (cmd.getParams().empty())
		Messages::needMoreParams(*user, cmd);
	else
	{
		std::string channelName = cmd.getParams()[0];
		if (channelName == "0")
			user->leaveAllChannels();
		else if (!Channel::isChannelNameValid(channelName))
		{
			// Error: invalid channel name
			std::cerr << "JOIN: invalid channel name: " << channelName << std::endl;
		}
		else
		{
			const std::map<std::string, Channel>::iterator &channel = _channels.find(channelName);
			if (channel != _channels.end())
			{
				// bool passwordOk = (channel->second.isPasswordMode() && cmd.getParams().size() == 2 &&
				// 				   channel->second.isPasswordValid(cmd.getParams()[1])) &&
				// 				  !channel->second.isPasswordMode();
				bool inviteOk = (channel->second.isInviteMode() && channel->second.isInvited(user)) ||
								!channel->second.isInviteMode();

				// if (passwordOk && inviteOk)
				if (inviteOk)
					user->joinChannel(&channel->second); //TODO: send all confirmation message when the channel is joined
				else if (!inviteOk)
					Messages::cannotJoinInvite(*user, channelName);
				else
					Messages::cannotJoinPassowrd(*user, channelName);
			}
			else
			{
				Channel newChannel(user, channelName);
				_channels.insert(std::make_pair(channelName, newChannel));
				newChannel.addUser(user);
				// user->sendMessage(user->getNickname() + "JOIN" + channelName);
			}
		}
	}
}

void Server::cmdInvite(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() != 2)
		Messages::needMoreParams(*user, cmd);
	else
	{
		std::string channelName = cmd.getParams()[1];
		const std::map<std::string, Channel>::iterator &channel = _channels.find(channelName);
		if (channel == _channels.end())
			Messages::noSuchChannel(*user, channelName);
		else if (!channel->second.isInviteMode())
			return; //TODO
		else if (!channel->second.isUserOnChannel(user))
			Messages::notOnChannel(*user, channelName);
		else if (!channel->second.isUserOperator(user))
			Messages::notOperator(*user, channelName);
		else
		{
			for (std::map<int, User>::iterator it = _clients.begin(); it != _clients.end(); it++)
			{
				if (it->second.getNickname() == cmd.getParams()[0])
				{
					if (channel->second.isUserOnChannel(&(it->second)))
						Messages::alreadyOnChannel(*user, channelName);
					else
						channel->second.inviteUser(&(it->second));
					break;
				}
			}
		}
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
