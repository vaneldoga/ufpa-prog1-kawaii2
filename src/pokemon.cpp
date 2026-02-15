#include "pokemon.hpp"
#include <iostream>

void
Pokemon::print
(void)
{
	std::cout << "id: " << global_id << ";" << std::endl;
	std::cout << "name: " << name << ";" << std::endl;
	std::cout << "evolution stage: " << evolution_stage << ";" << std::endl;
	std::cout << "generation: " << generation << ";" << std::endl;
	std::cout << "weight: " << weight << ";" << std::endl;
}