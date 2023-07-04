#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include "Channel.hpp"

class Channel;

class User
{
public:
	explicit User(int socket);

	virtual ~User();

	int getSocket() const;

	const std::string &getNickname() const;

	Channel *getConnectedChannel() const;

private:
	int			_socket;
	std::string	_nickname;
	Channel 	*_connectedChannel;
};


#endif //USER_HPP
