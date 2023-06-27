//#include "User.hpp"
//#include "Channel.hpp"
#include "utils.hpp"

#include <iostream>
#include <sstream>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

static int getPort(std::string str);

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Invalid use, use ft_irc <port number> <password>" << std::endl;
		return (1);
	}

	int port = getPort(argv[1]);
	if (port == -1)
	{
		std::cerr << "Invalid port: " << BOLD << argv[1] << std::endl;
		return (1);
	}
	std::cout << port << std::endl;

	std::string password = std::string(argv[2]);

	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "Error while creating socket" << std::endl;
		return (1);
	}
	sockaddr_in serverAddress = {};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

}

static int getPort(std::string str)
{
	for (std::string::iterator c = str.begin(); c < str.end(); c++)
	{
		if (!std::isdigit(*c))
			return -1;
	}
	int value = 0;
	std::istringstream s(str);
	s >> value;
	return (static_cast<int>(value));
}