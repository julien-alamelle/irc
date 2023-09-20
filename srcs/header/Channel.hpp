//
// Created by Marius  on 04/07/2023.
//

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <iostream>
#include "User.hpp"

class User;

class Channel
{
public:
	explicit Channel(User *creator);

	virtual ~Channel();

	void	addUser(User *user);
	void	delUser(User *user);
	bool	isOperator(User *user);
	void	setInviteMode(bool mode);
	void	setTopicMode(bool mode);
	void	setPassword(std::string key);
	void	setOperator(User *user, bool status);
	void	setUserLimit(int mode);

private:
	std::vector<User *>	_connectedUsers;
	std::vector<User *>	_operators;
	std::string			_topic;
	bool				_inviteMode;
	bool				_topicMode;
	bool				_passwordMode;
	std::string			_password;
	int					_userLimit;
};


#endif //CHANNEL_HPP
