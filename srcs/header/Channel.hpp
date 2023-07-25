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

	void addUser(User *user);
	void removeUser(User *user);

	bool isInviteMode() const;
	bool isPasswordMode() const;
	bool isPasswordValid(const std::string& password);
	bool isInvited(User *user);

	static bool isChannelNameValid(std::string& name);

private:
	std::vector<User *>	_connectedUsers;
	std::vector<User *>	_invitedUsers;
	std::vector<User *>	_operators;
	std::string			_topic;
	bool				_inviteMode;
	bool				_passwordMode;
	std::string			_password;

};


#endif //CHANNEL_HPP
