#include <string>
#include <vector>

class Pokemon
{
public:
	unsigned int global_id; // ID in global pokedéx.
	std::string name;
	std::string ability;
	unsigned int evolution_stage; // from 1 onwards
	float weigth; // in lbs. TODO: convertable to kg
	unsigned int generation; // generation that introduced this pokemon
	std::vector<std::string> weaknesses;
	std::vector<std::string> strengths;
	std::vector<std::string> immunities;
	std::vector<std::string> egg_group;

	bool
	is_ready
	(void);

	// Parses a key-value formatted string.
	//
	// One key-value pair per line, blank line separating pokemons.
	//
	// Square brackets for lists.
	//
	// Example:
	//     id: 45
	//     name: vileplume
	//     ability: chlorophyll
	//     evolution stage: 3
	//     weight: 41.0
	//     generation: 1
	//     weaknesses: [fire, ice, flying, psychic]
	//
	// I'm prevented to use json or something alike :/
	bool
	parse_string
	(std::string);
};