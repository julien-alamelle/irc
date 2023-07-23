//
// Created by Marius  on 23/07/2023.
//

#include "Messages.hpp"


void Messages::cannotJoinInvite(std::string userName, std::string channelName, int socket)
{
	std::string errorMsg = userName + " " + channelName + " :Cannot join channel (+i)";
	send(socket, errorMsg.c_str(), errorMsg.size(), 0);
}

void Messages::cannotJoinPassowrd(std::string userName, std::string channelName, int socket)
{
	std::string errorMsg = userName + " " + channelName + " :Cannot join channel (+k)";
	send(socket, errorMsg.c_str(), errorMsg.size(), 0);
}
