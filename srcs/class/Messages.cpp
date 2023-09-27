//
// Created by Marius  on 23/07/2023.
//

#include "Messages.hpp"
#include "Server.hpp"


void Messages::cannotJoinInvite(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :Cannot join channel (+i)";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::cannotJoinPassowrd(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :Cannot join channel (+k)";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::needMoreParams(const User &user, const Commande& cmd)
{
	std::string errorMsg = user.getNickname() + " " + cmd.getCommande() + " :Not enough parameter";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::incorrectPassword(const User &user)
{
	std::string errorMsg = user.getNickname() + " :Password incorrect";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::alreadyRegistered(const User &user)
{
	std::string errorMsg = user.getNickname() + " :You may not reregister";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::invalidNickName(const User &user)
{
	std::string errorMsg = user.getNickname() + " :Erroneus nickname";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::noSuchChannel(const User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :No such channel";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::notOnChannel(const User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :You're not on that channel";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::notOperator(const User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :You're not channel operator";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::alreadyOnChannel(const User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :is already on channel";
	Server::ft_send(user.getSocket(), errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}
