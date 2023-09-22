#pragma once

#include <string>
#include <vector>

class Commande {
private:
	std::string		prefix;
	std::string		commande;
	std::vector<std::string>		params;

public:
	Commande();
	Commande(const Commande &);
	virtual ~Commande();
	Commande&	operator=(const Commande &);

	void parse(std::string);
	std::string toString();

	void setPrefix(std::string);
	const std::string &getPrefix() const;
	const std::string &getCommande() const;
	const std::vector<std::string> &getParams() const;
	std::vector<std::string> &getParams();
};
