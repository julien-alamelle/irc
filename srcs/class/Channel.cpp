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

void	Channel::addUser(User *user, std::string key)
{
	if (this->_userLimit >= 0 && static_cast<int>(this->_connectedUsers.size()) >= this->_userLimit)
		return;
	if (this->_passwordMode && key != this->_password)
		return;
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		if (user == *it)
		{
			return;
		}
	}
	if (this->_inviteMode)
	{
		for (vecusit it = this->_invites.begin(); it < this->_invites.end(); ++it)
		{
			if (user == *it)
			{
				this->_invites.erase(it);
				break;
			}
		}
		return;
	}
	this->_connectedUsers.push_back(user);
}

void	Channel::inviteUser(User *user)
{
	if (!this->_inviteMode)
		return;
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		if (user == *it)
			return;
	}
	for (vecusit it = this->_invites.begin(); it < this->_invites.end(); ++it)
	{
		if (user == *it)
			return;
	}
	this->_invites.push_back(user);
}

void	Channel::delUser(User *user)
{
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		if (user == *it)
		{
			this->_connectedUsers.erase(it);
			this->setOperator(user, false);
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

void	Channel::setOperator(User *user, bool status)
{
	if (status)
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
	else
	{
		for (vecusit it = this->_operators.begin(); it < this->_operators.end(); ++it)
		{
			if (user == *it)
				this->_operators.erase(it);
		}
	}
}

void	Channel::setUserLimit(int mode)
{
	this->_userLimit = mode;
}
