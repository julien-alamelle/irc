//
// Created by Marius  on 04/07/2023.
//

#include "Channel.hpp"

Channel::Channel(User *creator)
{
	_connectedUsers.push_back(creator);
	_operators.push_back(creator);
	_topic = "";
	_inviteMode = false;
	_passwordMode = false;
	_password = "";
}

Channel::~Channel()
{
	std::cout << "Channel deleted" << std::endl;
}
