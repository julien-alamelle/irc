#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

#include "../header/User.hpp"

class Server
{
public:
	Server(int port, const std::string& password);
	void start(int &keep);

private:
	std::vector<pollfd>	_clientSockets;
	std::map<int, User>	_clients;
	sockaddr_in			_serverAddress;
	int					_serverSocket;
	int					_port;
	std::string			_password;

	void newConnexion();
	void newMessage(int clientNumber);
	void disconnexion(int clientNumber);
	void handleMessage(char *buffer, int clientNumber);

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
