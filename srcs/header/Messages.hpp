//
// Created by Marius  on 23/07/2023.
//

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <netinet/in.h>

#include "Commande.hpp"
#include "User.hpp"

class Messages
{
public:
	//TODO: Change userName in parameters, its not what's required
	static void cannotJoinInvite(const User &user, const std::string& channelName);
	static void cannotJoinPassowrd(const User &user, const std::string& channelName);
	static void needMoreParams(const User &user, const Commande& cmd);
	static void incorrectPassword(const User &user);
	static void alreadyLogged(const User &user);
	static void invalidNickName(const User &user);
};


#endif //MESSAGES_HPP
