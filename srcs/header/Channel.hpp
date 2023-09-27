//
// Created by Marius  on 04/07/2023.
//

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <iostream>
#include <algorithm>

class User;

class Channel
{
public:
	explicit Channel(User *creator, const std::string &name);

	virtual ~Channel();

	bool	addUser(User *user, std::string key);
	int		inviteUser(User *user);
	void	delUser(User *user);
	bool	isOperator(const User *user);

	std::string	getTopic();
	bool	getTopicMode();
//	std::string	getName();

	void	setTopic(std::string);
	void	setInviteMode(bool mode);
	void	setTopicMode(bool mode);
	void	setPassword(std::string key);
	void	setOperator(User *user, bool status);
	void	setUserLimit(int mode);
	std::vector<User *>::iterator uBegin();
	std::vector<User *>::iterator uEnd();

	bool isEmpty();

	void removeUser(User *user);


	bool isUserOnChannel(const User *user);

	bool isInviteMode() const;
	bool isPasswordMode() const;
	bool isPasswordValid(const std::string& password);
	bool isInvited(User *user);

	static bool isChannelNameValid(std::string& name);

private:
	std::vector<User *>	_connectedUsers;
	std::vector<User *>	_invitedUsers;
	std::vector<User *>	_operators;
	std::vector<User *>	_invites;
	std::string			_name;
	std::string			_topic;
	bool				_inviteMode;
	bool				_topicMode;
	bool				_passwordMode;
	std::string			_password;
	int					_userLimit;

};


#endif //CHANNEL_HPP
