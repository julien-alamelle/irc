//
// Created by Marius  on 23/07/2023.
//

#include "Messages.hpp"


void Messages::cannotJoinInvite(const std::string& userName, const std::string& channelName, int socket)
{
	std::string errorMsg = userName + " " + channelName + " :Cannot join channel (+i)";
	send(socket, errorMsg.c_str(), errorMsg.size(), 0);
}

void Messages::cannotJoinPassowrd(const std::string& userName, const std::string& channelName, int socket)
{
	std::string errorMsg = userName + " " + channelName + " :Cannot join channel (+k)";
	send(socket, errorMsg.c_str(), errorMsg.size(), 0);
}

void Messages::needMoreParams(const std::string &userName, const Commande& cmd, int socket)
{
	std::string errorMsg = userName + " " + cmd.getCommande() + " :Not enough parameter";
	send(socket, errorMsg.c_str(), errorMsg.size(), 0);
}

void Messages::incorrectPassword(const std::string &userName, int socket)
{
	std::string errorMsg = userName + " :Password incorrect";
	send(socket, errorMsg.c_str(), errorMsg.size(), 0);
}

void Messages::alreadyLogged(const std::string &userName, int socket)
{
	std::string errorMsg = userName + " :You may not reregister";
	send(socket, errorMsg.c_str(), errorMsg.size(), 0);
}
