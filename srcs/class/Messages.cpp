//
// Created by Marius  on 23/07/2023.
//

#include "Messages.hpp"


void Messages::cannotJoinInvite(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getUsername() + " " + channelName + " :Cannot join channel (+i)";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	DEBUG ? std::cerr << errorMsg << std::endl: 0;
}

void Messages::cannotJoinPassowrd(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getUsername() + " " + channelName + " :Cannot join channel (+k)";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	DEBUG ? std::cerr << errorMsg << std::endl: 0;
}

void Messages::needMoreParams(const User &user, const Commande& cmd)
{
	std::string errorMsg = user.getUsername() + " " + cmd.getCommande() + " :Not enough parameter";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	DEBUG ? std::cerr << errorMsg << std::endl: 0;
}

void Messages::incorrectPassword(const User &user)
{
	std::string errorMsg = user.getUsername() + " :Password incorrect";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	DEBUG ? std::cerr << errorMsg << std::endl: 0;
}

void Messages::alreadyRegistered(const User &user)
{
	std::string errorMsg = user.getUsername() + " :You may not reregister";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	DEBUG ? std::cerr << errorMsg << std::endl: 0;
}

void Messages::invalidNickName(const User &user)
{
	std::string errorMsg = user.getNickname() + " :Erroneus nickname";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	DEBUG ? std::cerr << errorMsg << std::endl: 0;
}
