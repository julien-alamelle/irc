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

User::User(int socket) : _socket(socket), _passwordOk(false)
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
	_connectedChannels.push_back(channel);
	channel->addUser(this);
}

void User::leaveChannel(Channel *channel)
{
	std::vector<Channel *>::iterator chan = std::find(_connectedChannels.begin(), _connectedChannels.end(), channel);
	if (chan != _connectedChannels.end())
		_connectedChannels.erase(chan);
}

void User::leaveAllChannels()
{
	for (std::vector<Channel *>::iterator it = _connectedChannels.begin(); it < _connectedChannels.end(); it++)
	{
		(*it)->removeUser(this);
		_connectedChannels.erase(it);
	}
}


/* EXCEPTIONS */

const char *User::InvalidNick::what() const throw()
{
	return "Invalid nickname";
}
