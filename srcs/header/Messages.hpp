//
// Created by Marius  on 23/07/2023.
//

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <netinet/in.h>

class Messages
{
public:
	static void cannotJoinInvite(std::string userName, std::string channelName, int socket);
	static void cannotJoinPassowrd(std::string userName, std::string channelName, int socket);
};


#endif //MESSAGES_HPP
