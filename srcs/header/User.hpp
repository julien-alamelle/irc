#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <map>
#include <vector>

class Channel;

typedef std::map<std::string, Channel>::iterator mscit;

class User
{
public:
	explicit User(int socket);

	virtual ~User();

	int		getSocket() const;

	const std::string &getNickname() const;
	void	setNickname(const std::string &nickname);

	bool	isPasswordOk() const;
	void	setPasswordOk();

	const std::string &getUsername() const;
	void	setUsername(const std::string &username);

	const std::string &getRealname() const;
	void	setRealname(const std::string &realname);

	void	addMessage(std::string msg);
	std::vector<std::string> &getMessages();

	void	joinChannel(mscit it);
	void	leaveChannel(mscit it);
	mscit	getChannel();

private:
	const int					_socket;
	std::string					_nickname;
	std::string					_username;
	std::string					_realname;
	std::vector<mscit>			_connectedChannels;
	std::vector<std::string>	_toSend;
	bool						_passwordOk;


/* EXCEPTIONS */
public:
	class InvalidNick: public std::exception {
		virtual const char*	what() const throw();
	};

	class NoChannel: public std::exception {
		virtual const char*	what() const throw();
	};
};


#endif //USER_HPP
