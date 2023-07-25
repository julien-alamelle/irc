#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <cstring>
#include "Channel.hpp"

class Channel;

class User
{
public:
	explicit User(int socket);

	virtual ~User();

	int getSocket() const;

	Channel *getConnectedChannel() const;

	const std::string &getNickname() const;
	void setNickname(const std::string &nickname);

	bool	isPasswordOk() const;
	void	setPasswordOk();

	const std::string &getUsername() const;
	void setUsername(const std::string &username);

	const std::string &getRealname() const;
	void setRealname(const std::string &realname);

	void joinChannel(Channel *channel);
	void leaveChannel();

private:
	const int	_socket;
	std::string	_nickname;
	std::string	_username;
	std::string	_realname;
	Channel 	*_connectedChannel;
	bool		_passwordOk;


/* EXCEPTIONS */
public:
	class InvalidNick: public std::exception {
		virtual const char*	what() const throw();
	};
};


#endif //USER_HPP
