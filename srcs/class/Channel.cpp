//
// Created by Marius  on 04/07/2023.
//

#include "Channel.hpp"
typedef std::vector<User *>::iterator vecusit;

Channel::Channel(User *creator, const std::string &name)
{
	_connectedUsers.push_back(creator);
	_operators.push_back(creator);
	_topic = "";
	_inviteMode = false;
	_topicMode = true;
	_passwordMode = false;
	_password = "";
	_name = name;
	_userLimit = -1;
}

Channel::~Channel()
{}

bool	Channel::addUser(User *user, std::string key)
{
	if (this->_userLimit >= 0 && static_cast<int>(this->_connectedUsers.size()) >= this->_userLimit)
	{
		std::cout << "too many user\n";
		return false;
	}
	if (this->_passwordMode && key != this->_password)
	{
		std::cout << "bad pass\n";
		return false;
	}
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
		if (user == *it)
	{
		std::cout << "already in\n";
			return false;
	}
	if (this->_inviteMode)
	{
		vecusit it;
		for (it = this->_invites.begin(); it < this->_invites.end(); ++it)
		{
			if (user == *it)
			{
				this->_invites.erase(it);
				for (std::vector<User *>::iterator it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
				{
					user->sendMessage(":" + user->getNickname() + " JOIN " + _name); // :<nickname du user> JOIN <nom channel>
					std::cout << "addUser" << std::endl;
				}
				this->_connectedUsers.push_back(user);
				return true;
			}
		}
		std::cout << "not invited\n";
		return false;
	}
	for (std::vector<User *>::iterator it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		user->sendMessage(":" + user->getNickname() + " JOIN " + _name); // :<nickname du user> JOIN <nom channel>
		std::cout << "addUser" << std::endl;
	}
	this->_connectedUsers.push_back(user);
	return true;
}

void	Channel::inviteUser(User *user)
{
	if (!this->_inviteMode)
	{
		std::cout << "not invite mode\n";
		return;
	}
	for (vecusit it = this->_connectedUsers.begin(); it < this->_connectedUsers.end(); ++it)
	{
		if (user == *it)
		{
		std::cout << "already in\n";
			return;
		}
	}
	for (vecusit it = this->_invites.begin(); it < this->_invites.end(); ++it)
	{
		if (user == *it)
		{
		std::cout << "already invited\n";
			return;
		}
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

bool	Channel::isOperator(const User *user)
{
	for (vecusit it = this->_operators.begin(); it < this->_operators.end(); ++it)
		if (user == *it)
			return true;
	return false;
}

std::string	Channel::getTopic()
{
	return this->_topic;
}

bool	Channel::getTopicMode()
{
	return this->_topicMode;
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
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

bool Channel::isUserOnChannel(const User *user)
{
	for (std::vector<User *>::iterator it = _connectedUsers.begin(); it < _connectedUsers.end() ;it++)
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

vecusit Channel::uBegin()
{
	return this->_connectedUsers.begin();
}

vecusit Channel::uEnd()
{
	return this->_connectedUsers.end();
}
