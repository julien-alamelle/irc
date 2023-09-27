#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <algorithm>

#include "User.hpp"
#include "Channel.hpp"
#include "Commande.hpp"
#include "Messages.hpp"

#define GREEN "\e[32m"
#define CYAN "\e[34m"
#define RED "\e[31m"
#define END "\e[0m"

class Server
{
public:
	Server(int port, const std::string& password);
	void start(int &keep);

private:
	std::vector<pollfd>									_clientSockets;
	std::map<int, User>									_clients;	//fd -> User
	std::map<std::string, Channel>						_channels;	//Name -> Channel
	sockaddr_in											_serverAddress;
	int													_serverSocket;
	int													_port;
	std::string											_password;


	User* findUser(std::string);

	void newConnexion();
	void newMessage(std::vector<pollfd>::iterator it);
	void disconnexion(std::vector<pollfd>::iterator it);
	void handleMessage(char *buffer, std::vector<pollfd>::iterator it);
	void delUserFromChannel(std::map<std::string, Channel>::iterator it, User *user);

	/* COMMANDS */
	void cmdUser(const Commande& cmd, User *user);
	void cmdNick(const Commande& cmd, User *user);
	void cmdMode(const Commande& cmd, User *user);
	void cmdJoin(const Commande& cmd, User *user);
	void cmdInvi(const Commande& cmd, User *user);
	void cmdPMSG(const Commande& cmd, User *user);
	void cmdPass(const Commande& cmd, User *user);
	void cmdKick(const Commande& cmd, User *user);
	void cmdTopi(const Commande& cmd, User *user);
	void cmdQuit(const Commande& cmd, User *user);

/* EXCEPTIONS */
public:
	class SocketError: public std::exception {
		virtual const char*	what() const throw();
	};

	class BindError: public std::exception {
		virtual const char*	what() const throw();
	};

	class ListenError: public std::exception {
		virtual const char*	what() const throw();
	};

	class PollError: public std::exception {
		virtual const char*	what() const throw();
	};

	class ConnexionError: public std::exception {
		virtual const char*	what() const throw();
	};

};

#endif //SERVER_HPP
