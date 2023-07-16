#include "User.hpp"

User::User(int socket) : _socket(socket), _connectedChannel(NULL)
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

Channel *User::getConnectedChannel() const
{
	return _connectedChannel;
}

void User::setPasswordOk()
{
	_passwordOk = true;
}

bool User::isPasswordOk() const
{
	return _passwordOk;
}
