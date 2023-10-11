#include <poll.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Commande.hpp"

std::string getSubstr(std::string &str)
{
	std::string ret;
	size_t i = str.find_first_of("\n\r");
	while (i == 0)
	{
		str.erase(str.begin());
		i = str.find_first_of("\n\r");
	}
	if (i != std::string::npos)
	{
		ret = str.substr(0, i);
		str.erase(0,i);
	}
	return ret;
}

void sendMsg(std::string msg, int fd = -1)
{
	static std::vector<std::string> toSend;
	if (!msg.empty())
		toSend.push_back(msg);
	else
	{
		if (fd >= 0)
		{
			for (std::vector<std::string>::iterator it = toSend.begin(); it < toSend.end(); ++it)
				send(fd, it->c_str(), it->size(), 0);
		}
		toSend.clear();
	}
}


bool handleMessage(int fd)
{
	static std::string s;
	char buff[1025];
	int i = recv(fd, buff, 1024, 0);
	if (i > 0)
	{
		buff[i] = 0;
		s = s + buff;
	}
	std::string line = getSubstr(s);
	while (!line.empty())
	{
		Commande cmd;
		cmd.parse(line);
		if (cmd.getCommande() == "464")
		{
			std::cerr << "bad pass\n";
			return false;
		}
		else if (cmd.getCommande() == "433")
		{
			std::cerr << "nickname already in use\n";
			return false;
		}
		else if (cmd.getCommande() == "INVITE")
		{
			sendMsg("JOIN " + cmd.getParams().at(1));
		}
		else if (cmd.getCommande() == "JOIN")
		{
			sendMsg("PRIVMSG " + cmd.getParams().at(0) + " :Welcome in " + line.substr(1, line.find(' ')));
		}
		line = getSubstr(s);
	}
	return true;
}
/*	msg = "PRIVMSG #test :you suck\r\n";
	send(sock, msg.c_str(), msg.size(), 0);*/

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Usage: ./ircbot <port> <pass>\n";
		return 0;
	}
	int port = atoi(av[1]);
	std::string pass(av[2]);
	std::string msg;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	addr.sin_port = htons(port);
	connect(sock, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
	sendMsg("PASS " + pass + "\r\n");
	sendMsg("NICK ircbot\r\n");
	sendMsg("USER ircbot 0 * :ircbot\r\n");
	struct pollfd srv;
	srv.fd = sock;
	srv.events = POLLIN | POLLOUT;
	bool keep = true;
	while (keep && poll(&srv,1,-1) > 0) {
		if (srv.revents & (POLLERR | POLLHUP))
			keep = false;
		else if (srv.revents & POLLIN)
			keep = handleMessage(sock);
		else if (srv.revents & POLLOUT)
			sendMsg("",sock);
	}
	close(sock);
	sendMsg("");
}
