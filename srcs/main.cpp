//#include "header/User.hpp"
//#include "header/Channel.hpp"
#include "header/Server.hpp"
#include "header/utils.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <signal.h>

static int getPort(std::string str);
static void signalHandler(int signum);

int keep = 42;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Invalid use, use ft_irc <port> <password>" << std::endl;
		return (1);
	}

	int port = getPort(argv[1]);
	if (port == -1)
	{
		std::cerr << "Invalid port: " << BOLD << argv[1] << "." << std::endl;
		return (1);
	}

	std::string password = std::string(argv[2]);

	signal(SIGINT, signalHandler);

	try
	{
		Server server = Server(port, password);
		server.start(keep);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
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

static void signalHandler(int signum)
{
	if (signum == SIGINT || signum == SIGQUIT)
	{
		std::cout << std::endl << "Stopping server (signal " << signum << ")." << std::endl;
		keep = 0;
	}
}