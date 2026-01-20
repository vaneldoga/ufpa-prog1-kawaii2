#include <string>
#include <vector>

#include "pokemon.hpp"

class Pokedex
{
protected:
	std::vector<Pokemon> pokemons;

public:
	enum class SortType
	{
		ID,
		EVOLTUION_STAGE,
		GENERATION,
		WEIGTH
	};

	int
	size
	(void);

	Pokemon
	get_pokemon
	(int index);

	// Validates and inserts pokemon.
	bool
	add_pokemon
	(Pokemon pokemon);

	bool
	delete_pokemon_by_global_id
	(unsigned int id);

	bool
	save_to_file
	(std::string filename);

	bool
	load_from_file
	(std::string filename);

	void
	sort
	(SortType type);
};