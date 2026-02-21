#include <string>
#include <memory>

#include "pokedex_pokemon_entry.hpp"
#include "pokemon.hpp"

class Pokedex
{
protected:
	std::shared_ptr<PokedexPokemonEntry> first_pokemon_entry;
	int size = 0;

public:
	enum class SortType
	{
		ID,
		EVOLTUION_STAGE,
		GENERATION,
		WEIGHT
	};

	int
	get_size
	(void);

	// Get by position
	Pokemon
	at
	(int position);

	// Get by ID
	Pokemon
	get_by_id
	(unsigned int id);

	// Get Interval
	void
	print_range
	(int start, int end);

	// Get by name
	Pokemon
	get
	(std::string name);

	// Validates and inserts pokemon.
	bool
	add
	(Pokemon pokemon);

	// Removes by name
	bool
	remove
	(std::string name);

	// Finds the entry with a given name and updates it with the new value.
	bool
	update
	(std::string name, Pokemon new_value);

	// Prints in a readable format.
	void
	print
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
