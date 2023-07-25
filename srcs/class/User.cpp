#include "User.hpp"

static bool isValidNickname(std::string nick)
{
	std::string::iterator	it = nick.begin();

	if (nick.empty())
		return false;
	while (it != nick.end() && (std::isalnum(*it) || strchr("|{}[]\\", *it)))
		it++;
	if (nick.end() == it)
		return true;
	return false;
}

User::User(int socket) : _socket(socket), _connectedChannel(NULL), _passwordOk(false)
{

}

User::~User()
{
	std::cout << "User deleted: " << _socket << std::endl;
}

int User::getSocket() const
{
	return _socket;
}

Channel *User::getConnectedChannel() const
{
	return _connectedChannel;
}

const std::string &User::getNickname() const
{
	return _nickname;
}

void User::setNickname(const std::string &nickname)
{
	if (isValidNickname(nickname))
		_nickname = nickname;
	else
		throw InvalidNick();
}

bool User::isPasswordOk() const
{
	return _passwordOk;
}

void User::setPasswordOk()
{
	_passwordOk = true;
}

const std::string &User::getUsername() const
{
	return _username;
}

void User::setUsername(const std::string &username)
{
	_username = username;
}

const std::string &User::getRealname() const
{
	return _realname;
}

void User::setRealname(const std::string &realname)
{
	_realname = realname;
}

void User::joinChannel(Channel *channel)
{
	//TODO: check if we need to remove it from other channels
	_connectedChannel = channel;
}

void User::leaveChannel()
{
	_connectedChannel = NULL;
}


/* EXCEPTIONS */

const char *User::InvalidNick::what() const throw()
{
	return "Invalid nickname";
}
