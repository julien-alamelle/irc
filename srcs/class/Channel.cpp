//
// Created by Marius  on 04/07/2023.
//

#include "Channel.hpp"
typedef std::vector<User *>::iterator vecusit;

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

void	Channel::addUser(User *user)
{
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		if (user == *it)
		{
			return;
		}
	}
	this->_connectedUsers.push_back(user);
}

void	Channel::delUser(User *user)
{
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		if (user == *it)
		{
			this->_connectedUsers.erase(it);
			this->unsetOperator(user);
			return;
		}
	}
}

bool	Channel::isOperator(User *user)
{
	for (vecusit it = this->_operators.begin(); it < this->_operators.end(); ++it)
	{
		if (user == *it)
			return true;
	}
	return false;
}

void	Channel::setInviteMode(bool mode)
{
	this->_inviteMode = mode;
}

void	Channel::setTopicMode(bool mode)
{
	this->_topicMode = mode;
}

void	Channel::setPassword(std::string key)
{
	this->_password = key;
	if (key.empty())
		this->_passwordMode = false;
	else
		this->_passwordMode = true;
}

void	Channel::setOperator(User *user)
{
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		if (user == *it)
		{
			this->_operators.push_back(user);
			return;
		}
	}
}

void	Channel::unsetOperator(User *user)
{
	for (vecusit it = this->_operators.begin(); it < this->_operators.end(); ++it)
	{
		if (user == *it)
			this->_operators.erase(it);
	}
}

void	Channel::setUserLimit(int mode)
{
	this->_userLimit = mode;
}
