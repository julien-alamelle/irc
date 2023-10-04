#include "Channel.hpp"
#include "User.hpp"
#include "Server.hpp"

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
	//std::cout << "User deleted: " << _socket << std::endl;
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

void User::addMessage(std::string msg)
{
	this->_toSend.push_back(msg);
}

std::vector<std::string> &User::getMessages()
{
	return this->_toSend;
}

void User::joinChannel(mscit it)
{
	_connectedChannels.push_back(it);
}

void User::leaveChannel(mscit it)
{
	std::vector<mscit>::iterator chan = std::find(_connectedChannels.begin(), _connectedChannels.end(), it);
	if (chan != _connectedChannels.end())
		_connectedChannels.erase(chan);
}

mscit User::getChannel()
{
	if (this->_connectedChannels.empty())
		throw NoChannel();
	return	this->_connectedChannels.back();
}


/* EXCEPTIONS */

const char *User::InvalidNick::what() const throw()
{
	return "Invalid nickname";
}

const char *User::NoChannel::what() const throw()
{
	return "No channel";
}
