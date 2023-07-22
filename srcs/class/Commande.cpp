#include "Commande.hpp"

Commande::Commande():prefix(),commande(),params() {;}
Commande::Commande(const Commande &src):prefix(src.prefix),commande(src.commande),params(src.params) {;}
Commande::~Commande() {;}

Commande&	Commande::operator=(const Commande &src) {
	if (this == &src) return (*this);
	this->prefix = src.prefix;
	this->commande = src.commande;
	this->params = src.params;
	return (*this);
}

typedef std::string::iterator strit;

strit	count(strit begin, strit end, bool space) {
	while (begin != end && (*begin != ' ' || !space))
		++begin;
	return begin;
}

void	Commande::parse(std::string CMD) {
	strit it = CMD.begin();
	strit tmp;
	//prefix
	if (*it == ':') {
		++it;
		tmp = count(it, CMD.end(), true);
		this->prefix = CMD.substr(it - CMD.begin(), tmp - it);
		it = tmp;
		while (*it == ' ')
			++it;
	}
	else this->prefix = std::string();
	//command
	tmp = count(it, CMD.end(), true);
	this->commande = CMD.substr(it - CMD.begin(), tmp - it);
	it = tmp;
	while (*it == ' ')
		++it;
	//param
	this->params.clear();
	while (it != CMD.end()) {
		if (*it == ':') tmp = count(it, CMD.end(), false);
		else tmp = count(it, CMD.end(), true);
		this->params.push_back(CMD.substr(it - CMD.begin(), tmp - it));
		it = tmp;
		while (*it == ' ')
			++it;
	}
}

std::string	Commande::toString() {
	return this->commande + "\n"; //Temporary
}



const std::string &Commande::getPrefix() const
{
	return prefix;
}

const std::string &Commande::getCommande() const
{
	return commande;
}

const std::vector<std::string> &Commande::getParams() const
{
	return params;
}
