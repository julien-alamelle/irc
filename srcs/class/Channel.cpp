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

void Channel::addUser(User *user)
{
	_connectedUsers.push_back(user);
}

bool Channel::isChannelNameValid(std::string &name)
{
	if (name.empty())
		return false;

	std::string::iterator	it = name.begin();

	if (*it != '#')
		return false;
	it++;
	while (it != name.end() && *it != ' ' && *it != ',' && *it != 0x07)
		it++;
	if (name.end() != it)
		return false;
	return true;
}

bool Channel::isInviteMode() const
{
	return _inviteMode;
}

bool Channel::isPasswordMode() const
{
	return _passwordMode;
}

bool Channel::isPasswordValid(const std::string &password)
{
	return password == _password;
}

bool Channel::isInvited(User *user)
{
	return (std::find(_invitedUsers.begin(), _invitedUsers.end(), user) != _invitedUsers.end());
}

