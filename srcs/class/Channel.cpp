//
// Created by Marius  on 04/07/2023.
//

#include "Channel.hpp"

Channel::Channel(User *creator, const std::string &name)
{
	_connectedUsers.push_back(creator);
	_operators.push_back(creator);
	_topic = "";
	_inviteMode = false;
	_passwordMode = false;
	_password = "";
	_name = name;
}

Channel::~Channel()
{
	std::cout << "Channel deleted" << std::endl;
}

void Channel::addUser(User *user)
{
	for (std::vector<User *>::iterator it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		user->sendMessage(":" + user->getNickname() + " JOIN " + _name); // :<nickname du user> JOIN <nom channel>
		std::cout << "addUser" << std::endl;
		if (user == *it)
			return;
	}
	_connectedUsers.push_back(user);
}

bool Channel::isEmpty()
{
	return _connectedUsers.empty();
}

void Channel::removeUser(User *user)
{
	for (std::vector<User *>::iterator it = _connectedUsers.begin(); it < _connectedUsers.end() ;it++)
		if (*it == user)
			_connectedUsers.erase(it);
	for (std::vector<User *>::iterator it = _operators.begin(); it < _operators.end() ;it++)
		if (*it == user)
			_operators.erase(it);
}

void Channel::inviteUser(User *user)
{
	if (std::find(_invitedUsers.begin(), _invitedUsers.end(), user) != _invitedUsers.end())

	_invitedUsers.push_back(user);
}

bool Channel::isUserOnChannel(const User *user)
{
	for (std::vector<User *>::iterator it = _connectedUsers.begin(); it < _connectedUsers.end() ;it++)
		if (*it == user)
			return true;
	return false;
}

bool Channel::isUserOperator(const User *user)
{
	for (std::vector<User *>::iterator it = _operators.begin(); it < _operators.end() ;it++)
		if (*it == user)
			return true;
	return false;
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
	for (std::vector<User *>::iterator it = _invitedUsers.begin(); it < _invitedUsers.end() ;it++)
		if ((*it)->getSocket() == user->getSocket())
			return true;

	return false;
}

