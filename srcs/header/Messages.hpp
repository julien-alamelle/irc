//
// Created by Marius  on 23/07/2023.
//

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <netinet/in.h>

#include "Commande.hpp"

class Messages
{
public:
	//TODO: Check if it need really need userName or something else like nickName or realName
	static void cannotJoinInvite(const std::string& userName, const std::string& channelName, int socket);
	static void cannotJoinPassowrd(const std::string& userName, const std::string& channelName, int socket);
	static void needMoreParams(const std::string& userName, const Commande& cmd, int socket);
	static void incorrectPassword(const std::string& userName, int socket);
	static void alreadyLogged(const std::string& userName, int socket);
};


#endif //MESSAGES_HPP
