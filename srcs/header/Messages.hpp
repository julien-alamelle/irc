//
// Created by Marius  on 23/07/2023.
//

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <netinet/in.h>

#include "Commande.hpp"
#include "User.hpp"

#define DEBUG 1

class Messages
{
public:
	static void cannotJoinInvite(User &user, const std::string& channelName);
	static void cannotJoinPassowrd(User &user, const std::string& channelName);
	static void needMoreParams(User &user, const Commande& cmd);
	static void incorrectPassword(User &user);
	static void alreadyRegistered(User &user);
	static void invalidNickName(User &user);
	static void noSuchChannel(User &user, const std::string& channelName);
	static void notOnChannel(User &user, const std::string& channelName);
	static void notOperator(User &user, const std::string& channelName);
	static void alreadyOnChannel(User &user, const std::string& channelName);
};


#endif //MESSAGES_HPP
