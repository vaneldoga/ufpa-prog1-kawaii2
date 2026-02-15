#include <string>
#include <memory>

#include "pokedex_pokemon_entry.hpp"
#include "pokemon.hpp"

class Pokedex
{
protected:
	std::shared_ptr<PokedexPokemonEntry> first_pokemon_entry;

public:
	enum class SortType
	{
		ID,
		EVOLTUION_STAGE,
		GENERATION,
		WEIGHT
	};

	int
	size
	(void);

	Pokemon
	get_by_name
	(std::string name);

	// Validates and inserts pokemon.
	bool
	add
	(Pokemon pokemon);

	bool
	remove
	(std::string name);

	// Prints in a readable format.
	void
	print_to_stdout
	(void);

	bool
	save_to_file
	(const char* filename);

	bool
	load_pokemon
	(std::string block);

	bool
	load_from_file
	(const char* filename);

	void
	sort
	(SortType type);
};