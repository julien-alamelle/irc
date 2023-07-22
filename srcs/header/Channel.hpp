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

private:
	std::vector<User *>	_connectedUsers;
	std::vector<User *>	_operators;
	std::string			_topic;
	bool				_inviteMode;
	bool				_passwordMode;
	std::string			_password;
};


#endif //CHANNEL_HPP
