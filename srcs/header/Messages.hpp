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
	static void cannotJoinInvite(const User &user, const std::string& channelName);
	static void cannotJoinPassowrd(const User &user, const std::string& channelName);
	static void needMoreParams(const User &user, const Commande& cmd);
	static void passwdMisMatch(const User &user);
	static void alreadyRegistered(const User &user);
	static void invalidNickName(const User &user);
	static void noSuchChannel(const User &user, const std::string& channelName);
	static void notOnChannel(const User &user, const std::string& channelName);
	static void notOperator(const User &user, const std::string& channelName);
	static void alreadyOnChannel(const User &user, const std::string& channelName);
};


#endif //MESSAGES_HPP
