#include "pokedex.hpp"

#include <fstream>

int
Pokedex::size
(void)
{ return pokemons.size(); }

Pokemon
Pokedex::get_pokemon
(int index)
{
	return pokemons[index];
}

bool
Pokedex::add_pokemon
(Pokemon pokemon)
{
	if (pokemon.is_ready())
	{
		pokemons.push_back(pokemon);
		printf("%s loaded to pokedex.\n", pokemon.name.c_str());
	}
	return false;
}

bool
Pokedex::delete_pokemon_by_global_id
(unsigned int id)
{
	(void)id;
	return false;
}

bool
Pokedex::save_to_file
(std::string filename)
{
	std::ofstream output_file(filename);
	if (!output_file.is_open())
	{
		fputs("Error saving to file. File not open.", stderr);
		return false;
	}
	output_file.close();
	return true;
}

bool
Pokedex::load_from_file
(std::string filename)
{
	std::ifstream source_file(filename);
	if (!source_file.is_open())
	{
		fputs("Error loading from file. File not open.", stderr);
		return false;
	}
	Pokemon pokemon;
	std::string line;
	while (std::getline(source_file, line))
	{
		size_t key_value_separator_position = line.find(":");
		if (key_value_separator_position == std::string::npos)
		{
			add_pokemon(pokemon);
			//pokemon.clear();
		}
		else
		{
			std::string key = line.substr(0, key_value_separator_position);
			std::string value = line.substr( line.find_first_not_of(" ", key_value_separator_position+1) );
			if (key == "name")
			{ pokemon.name = value; }
			else
			{ fprintf(stderr, "Unsupported key: %s.\n", key.c_str()); }
		}
	}
	add_pokemon(pokemon); // flush
	source_file.close();
	return true;
}

void
Pokedex::sort
(SortType type)
{
	(void)type;
	return;
}