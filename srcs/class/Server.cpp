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
		Messages::incorrectPassword(*user);
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

static void modeReply(Channel *chan, Commande &cmd, bool mode, char c, std::string arg)
{
	if (mode)
		cmd.getParams().at(1) = "+";
	else
		cmd.getParams().at(1) = "-";
	cmd.getParams().at(1).push_back(c);
	cmd.getParams().at(2) = arg;
	std::string reply = cmd.toString();
	std::cout << reply;
	for (std::vector<User *>::iterator it = chan->uBegin(); it < chan->uEnd(); ++it)
	{
		send((*it)->getSocket(), reply.c_str(), reply.size(), 0);
	}
}

void Server::cmdMode(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() < 2)
	{
		std::cout << "MODE: incorrect number of args " << cmd.getParams().size() << std::endl;
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	if (channelIT == this->_channels.end())
	{
		std::cout << "MODE: the channel does not exist " << cmd.getParams().at(0) << std::endl;
		return;
	}
	if (!channelIT->second.isOperator(user))
	{
		std::cout << "MODE: user is not operator" << std::endl;
		return;
	}
	unsigned long nbARG = 2;
	bool	mode = false;
	std::string modes = cmd.getParams().at(1);
	if (modes.length() <= 1 || (modes[0] != '+' && modes[0] != '-'))
	{
		std::cout << "MODE: invalide option " << modes << std::endl;
		return;
	}
	Commande reply(cmd);
	reply.getParams().resize(3);
	reply.setPrefix(":" + user->getNickname());
	if (modes[0] == '+') mode = true;
	std::string::iterator it = modes.begin();
	++it;
	while (it != modes.end())
	{
		switch (*it)
		{
		case 'i':
			channelIT->second.setInviteMode(mode);
			modeReply(&(channelIT->second), reply, mode, *it, "");
			break;

		case 't':
			channelIT->second.setTopicMode(mode);
			modeReply(&(channelIT->second), reply, mode, *it, "");
			break;

		case 'k':
			if (mode)
			{
				if (cmd.getParams().size() < nbARG + 1)
				{
					std::cout << "MODE: no matching argument with k" << std::endl;
					return;
				}
				channelIT->second.setPassword(cmd.getParams().at(nbARG));
				modeReply(&(channelIT->second), reply, mode, *it, cmd.getParams().at(nbARG));
				++nbARG;
			}
			else
			{
				channelIT->second.setPassword("");
				modeReply(&(channelIT->second), reply, mode, *it, "");
			}
			break;

		case 'o':
		{
			if (cmd.getParams().size() < nbARG + 1)
			{
				std::cout << "MODE: no matching argument with o" << std::endl;
				return;
			}
			User *opArg = this->findUser(cmd.getParams().at(nbARG));
			if (!opArg)
			{
				std::cout << "MODE: the user does not exist " << cmd.getParams().at(nbARG) << std::endl;
				return;
			}
			channelIT->second.setOperator(opArg, mode);
			modeReply(&(channelIT->second), reply, mode, *it, cmd.getParams().at(nbARG));
			++nbARG;
			break;
		}

		case 'l':
			if (mode)
			{
				if (cmd.getParams().size() < nbARG + 1)
				{
					std::cout << "MODE: no matching argument with l" << std::endl;
					return;
				}
				char *end;
				int lim = strtol(cmd.getParams().at(nbARG).c_str(), &end, 10);
				if (*end)
				{
					std::cout << "MODE: not a valid integer " << cmd.getParams().at(nbARG) << std::endl;
					return;
				}
				channelIT->second.setUserLimit(lim);
				modeReply(&(channelIT->second), reply, mode, *it, cmd.getParams().at(nbARG));
				++nbARG;
			}
			else
			{
				channelIT->second.setUserLimit(-1);
				modeReply(&(channelIT->second), reply, mode, *it, "");
			}
			break;

		default:
			std::cout << "MODE: invalide option " << modes << std::endl;
			return;
		}
		++it;
	}
}

void Server::cmdJoin(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() < 1 || cmd.getParams().size() > 2)
	{
		std::cout << "JOIN: incorrect number of args " << cmd.getParams().size() << std::endl;
		return;
	}
	if (cmd.getParams().at(0).at(0) != '#')
	{
		std::cout << "JOIN: invalide channel name " << cmd.getParams().at(0) << std::endl;
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	if (channelIT == this->_channels.end())
	{
		channelIT = this->_channels.insert(std::make_pair(cmd.getParams().at(0), Channel(user, cmd.getParams().at(0)))).first;
		if (cmd.getParams().size() == 2)
			channelIT->second.setPassword(cmd.getParams().at(1));
	}
	else
	{
		std::string key = "";
		if (cmd.getParams().size() == 2)
			key = cmd.getParams().at(1);
		if (!channelIT->second.addUser(user, key))
		{
			std::cout <<"JOIN: fail to join the channel\n";
			return;
		}
	}
	Commande ret(cmd);
	ret.setPrefix(":" + user->getNickname());
	std::string response = ret.toString();
	for (std::vector<User *>::iterator it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
	{
		send((*it)->getSocket(), response.c_str(), response.size(), 0);
	}
	std::string topic = channelIT->second.getTopic();
	if (topic.empty())
		response = ":" + user->getNickname() + " 331 " + user->getNickname() + " " + cmd.getParams().at(0) + " :No topic is set\r\n";
	else
		response = ":" + user->getNickname() + " 332 " + user->getNickname() + " " + cmd.getParams().at(0) + " :" + topic + "\r\n";
	send(user->getSocket(), response.c_str(), response.size(), 0);
	response = ":" + user->getNickname() + " 353 " + user->getNickname() + " = " + cmd.getParams().at(0) + " :";
	for (std::vector<User *>::iterator it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
	{
		if (channelIT->second.isOperator(*it))
			response += "@";
		response += (*it)->getNickname() + " ";
	}
	response += "\r\n";
	send(user->getSocket(), response.c_str(), response.size(), 0);
	response = ":" + user->getNickname() + " 366 " + user->getNickname() + " " + cmd.getParams().at(0) + " :End of /NAMES list\r\n";
	send(user->getSocket(), response.c_str(), response.size(), 0);
}

void Server::cmdInvi(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() != 2)
	{
		std::cout << "INVITE: incorrect number of args " << cmd.getParams().size() << std::endl;
		return;
	}
	User *invArg = this->findUser(cmd.getParams().at(0));
	if (!invArg)
	{
		std::cout << "INVITE: the user does not exist " << cmd.getParams().at(0) << std::endl;
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(1));
	if (channelIT == this->_channels.end())
	{
		std::cout << "INVITE: the channel does not exist " << cmd.getParams().at(1) << std::endl;
		return;
	}
	if (!(channelIT->second.isOperator(user)))
	{
		std::cout << "INVITE: user is not operator" << std::endl;
		return;
	}
	channelIT->second.inviteUser(invArg);
}

void Server::cmdPMSG(const Commande &cmd, User *user)
{
	if (cmd.getParams().size() != 2)
	{
		std::cout << "PRIVMSG: incorrect number of args " << cmd.getParams().size() << std::endl;
		return;
	}
	Commande ret(cmd);
	ret.setPrefix(":" + user->getNickname());
	User *msgArg = this->findUser(cmd.getParams().at(0));
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	std::string response = ret.toString();
	std::cout << "PRIVMSG responce: " << response;
	if (msgArg)
	{
		send(msgArg->getSocket(), response.c_str(), response.size(), 0);
	}
	else if (channelIT != this->_channels.end())
	{
		std::vector<User *>::iterator it;
		for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
			if (user == *it)
				break;
		if (it == channelIT->second.uEnd())
		{
			std::cout << "PRIVMSG: user is not a member of this channel\n";
			return;
		}
		for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
			if (user != *it)
				send((*it)->getSocket(), response.c_str(), response.size(), 0);
	}
	else
	{
		std::cout << "PRIVMSG: the user/channel does not exist " << cmd.getParams().at(0) << std::endl;
		return;
	}
}

void Server::cmdKick(const Commande &cmd, User *user) {(void) cmd; (void) user;}
void Server::cmdTopi(const Commande &cmd, User *user)
{
	std::string response;
	if (cmd.getParams().size() < 1)
	{
		response = ":" + user->getNickname() + " 461 " + user->getNickname() + " TOPIC :Not enought parameters\r\n";
		send(user->getSocket(), response.c_str(), response.size(), 0);
		return;
	}
	std::map<std::string, Channel>::iterator channelIT = this->_channels.find(cmd.getParams().at(0));
	if (channelIT == this->_channels.end())
	{
		response = ":" + user->getNickname() + " 403 " + user->getNickname() + " " + cmd.getParams().at(0) + ":No such channel\r\n";
		send(user->getSocket(), response.c_str(), response.size(), 0);
		return;
	}
	std::vector<User *>::iterator it;
	for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
		if (user == *it)
			break;
	if (it == channelIT->second.uEnd())
	{
		response = ":" + user->getNickname() + " 442 " + user->getNickname() + " " + cmd.getParams().at(0) + ":You're not on that channel\r\n";
		send(user->getSocket(), response.c_str(), response.size(), 0);
		return;
	}
	if (cmd.getParams().size() == 1)
	{
		std::string topic = channelIT->second.getTopic();
		if (topic.empty())
			response = ":" + user->getNickname() + " 331 " + user->getNickname() + " " + cmd.getParams().at(0) + " :No topic is set\r\n";
		else
			response = ":" + user->getNickname() + " 332 " + user->getNickname() + " " + cmd.getParams().at(0) + " :" + topic + "\r\n";
		send(user->getSocket(), response.c_str(), response.size(), 0);
		return;
	}
	if (channelIT->second.getTopicMode() && !channelIT->second.isOperator(user))
	{
		response = ":" + user->getNickname() + " 482 " + user->getNickname() + " " + cmd.getParams().at(0) + ":You're not channel operator\r\n";
		send(user->getSocket(), response.c_str(), response.size(), 0);
		return;
	}
	channelIT->second.setTopic(cmd.getParams().at(1).substr(1));
	Commande reply(cmd);
	reply.setPrefix(":" + user->getNickname());
	response = reply.toString();
	for (it = channelIT->second.uBegin(); it < channelIT->second.uEnd(); ++it)
		send((*it)->getSocket(), response.c_str(), response.size(), 0);
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
