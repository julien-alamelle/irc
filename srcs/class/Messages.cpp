//
// Created by Marius  on 23/07/2023.
//

#include "Messages.hpp"
#include "Server.hpp"


void Messages::cannotJoinInvite(User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :Cannot join channel (+i)\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::cannotJoinPassowrd(User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :Cannot join channel (+k)\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::needMoreParams(User &user, const Commande& cmd)
{
	std::string errorMsg = user.getNickname() + " " + cmd.getCommande() + " :Not enough parameter\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::incorrectPassword(User &user)
{
	std::string errorMsg = user.getNickname() + " :Password incorrect\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::alreadyRegistered(User &user)
{
	std::string errorMsg = user.getNickname() + " :You may not reregister\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::invalidNickName(User &user)
{
	std::string errorMsg = user.getNickname() + " :Erroneus nickname\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::noSuchChannel(User &user, const std::string& channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :No such channel\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::notOnChannel(User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :You're not on that channel\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::notOperator(User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :You're not channel operator\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}

void Messages::alreadyOnChannel(User &user, const std::string &channelName)
{
	std::string errorMsg = user.getNickname() + " " + channelName + " :is already on channel\n";
	user.addMessage(errorMsg);
	if (DEBUG)
		std::cerr << errorMsg << std::endl;
}
