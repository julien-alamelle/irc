#pragma once
#include <string>

class Client {
private:
	std::string		name;

public:
	Client();
	Client(std::string);
	Client(const Client &);
	virtual ~Client();
	Client&	operator=(const Client &);
};
