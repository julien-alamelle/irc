#include "Commande.hpp"
#include <iostream>

int main(int ac, char **av) {
	if (ac < 2)
		return 0;
	Commande test;
	test.parse(av[1]);
	std::cout << test.toString() << std::endl;
}
