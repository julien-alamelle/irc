#include "Client.hpp"

Client::Client():name() {;}
Client::Client(std::string name):name(name) {;}
Client::Client(const Client &src):name(src.name) {;}
Client::~Client() {;}

Client&	Client::operator=(const Client &src) {
	if (this == &src) return (*this);
	this->name = src.name;
	return (*this);
}
