//
// Created by Marius  on 23/07/2023.
//

#include "Messages.hpp"


void Messages::cannotJoinInvite(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :Cannot join channel (+i)";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::cannotJoinPassowrd(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :Cannot join channel (+k)";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::needMoreParams(const User &user, const Commande& cmd)
{
	std::string errorMsg = user.getNickname() + " " + cmd.getCommande() + " :Not enough parameter";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::passwdMisMatch(const User &user)
{
	std::string errorMsg = user.getNickname() + " :Password incorrect";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::alreadyRegistered(const User &user)
{
	std::string errorMsg = user.getNickname() + " :You may not reregister";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::invalidNickName(const User &user)
{
	std::string errorMsg = user.getNickname() + " :Erroneus nickname";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::noSuchChannel(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :No such channel";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::notOnChannel(const User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :You're not on that channel";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::notOperator(const User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :You're not channel operator";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::alreadyOnChannel(const User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :is already on channel";
	send(user.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}
