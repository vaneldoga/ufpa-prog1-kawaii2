#pragma once

#include <string>
#include <memory>

class Pokemon
{
public:
	unsigned int global_id; // ID in global pokedéx.
	std::string name;
	unsigned int evolution_stage; // from 1 onwards
	float weight; // in lbs. TODO: convertable to kg
	unsigned int generation; // generation that introduced this pokemon
	std::string abilities[5]; // we just assume 5 for now, since std::vector is prohibited
	std::string weaknesses[5];
	std::string strengths[5];
	std::string egg_group[5];

	void
	print
	(void);
};
