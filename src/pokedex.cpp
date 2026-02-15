#include "pokedex.hpp"

#include <fstream>
#include <iostream>

int
Pokedex::size
(void)
{
	int size = 0;
	std::shared_ptr<PokedexPokemonEntry> current_entry = first_pokemon_entry;
	while (current_entry != nullptr)
	{
		current_entry = current_entry->successor;
		size++;
	}
	return size;
}

Pokemon
Pokedex::get_by_name
(std::string name)
{
	Pokemon pokemon;
	pokemon.name = "";
	std::shared_ptr<PokedexPokemonEntry> current_entry = first_pokemon_entry;
	while (current_entry != nullptr && pokemon.name == "")
	{
		if (current_entry->pokemon.name == name)
		{ pokemon = current_entry->pokemon; }
		current_entry = current_entry->successor;
	}
	return pokemon;
}

bool
Pokedex::add
(Pokemon pokemon)
{
	std::shared_ptr<PokedexPokemonEntry> current_entry = first_pokemon_entry;
	if (current_entry == nullptr) {
		first_pokemon_entry = std::make_shared<PokedexPokemonEntry>();
		first_pokemon_entry->pokemon = pokemon;
		return true;
	}
	else
	{
		while (current_entry->successor != nullptr)
		{ current_entry = current_entry->successor; }
		current_entry->successor = std::make_shared<PokedexPokemonEntry>();
		current_entry->successor->pokemon = pokemon;
		current_entry->successor->predecessor = current_entry;
		return true;
	}
}

bool
Pokedex::remove
(std::string name)
{
	std::shared_ptr<PokedexPokemonEntry> current_entry = first_pokemon_entry;
	while (current_entry != nullptr)
	{
		if (current_entry->pokemon.name != name)
		{ current_entry = current_entry->successor; }
		else if (current_entry == first_pokemon_entry)
		{ first_pokemon_entry  = current_entry->successor; }
		else
		{
			std::shared_ptr<PokedexPokemonEntry> left_orphan = current_entry->predecessor.lock();
			std::shared_ptr<PokedexPokemonEntry> right_orphan = current_entry->successor;
			if (left_orphan != nullptr)
			{ left_orphan->successor = right_orphan; }
			if (right_orphan != nullptr)
			{ right_orphan->predecessor = left_orphan; }
			return true;
		}
	}
	return false;
}

void
Pokedex::print_to_stdout
(void)
{
	std::shared_ptr<PokedexPokemonEntry> current_entry = first_pokemon_entry;
	while (current_entry != nullptr)
	{
		current_entry->pokemon.print();
		std::cout << std::endl;
		current_entry = current_entry->successor;
	}
}

bool
Pokedex::save_to_file
(const char* filename)
{
	std::ofstream output_file(filename);
	if (!output_file.is_open())
	{
		fputs("Error saving to file. File not open.", stderr);
		return false;
	}
	PokedexPokemonEntry *current_entry = first_pokemon_entry.get();
	while (current_entry != NULL)
	{
		output_file << current_entry->pokemon.name;
		current_entry = current_entry->successor.get();
	}
	output_file.close();
	return true;
}

// Reads a string and extracts information of a pokemon
bool
Pokedex::load_pokemon
(std::string block)
{
	// Format: <field>: <value>; ...
	Pokemon pokemon;
	pokemon.name = "";
	size_t key_value_pair_boundaries[2] = {0, 0};
	bool keep_consuming_key_value_pairs = true;
	while (keep_consuming_key_value_pairs)
	{
		key_value_pair_boundaries[0] = block.find_first_not_of("; \n", key_value_pair_boundaries[1]);
		key_value_pair_boundaries[1] = block.find(";", key_value_pair_boundaries[0]);
		if (key_value_pair_boundaries[1] == std::string::npos)
		{
			if (pokemon.name == "")
			{ return false; }
			else
			{ add(pokemon); return true; }
		}
		else
		{
			size_t key_value_separator_position = block.find(":", key_value_pair_boundaries[0]);
			if (key_value_separator_position == std::string::npos)
			{
				perror("Syntax error. Missing key-value separator.\n");
				return false;
			}
			size_t key_length = key_value_separator_position - key_value_pair_boundaries[0];
			std::string key = block.substr(key_value_pair_boundaries[0], key_length);
			size_t value_length = block.find_last_not_of("; \n", key_value_pair_boundaries[1]) - key_value_separator_position - 1;
			std::string value = block.substr(
				block.find_first_not_of(" ", key_value_separator_position+1),
				value_length
			);
			//std::cout << key_value_pair_boundaries[0] << "-" << key_value_separator_position << "-" << key_value_pair_boundaries[1];
			//std::cout << "|" << key << "|" << value << "|" << std::endl;
			if (key == "id")
			{ pokemon.global_id = std::stoi(value); }
			else if (key == "name")
			{ pokemon.name = value; }
			else if (key == "evolution stage")
			{ pokemon.evolution_stage = std::stoi(value); }
			else if (key == "generation")
			{ pokemon.generation = std::stoi(value); }
			else if (key == "weight")
			{ pokemon.weight = std::stof(value); }
			else if (key == "abilities")
			{ pokemon.abilities[0] = value; }
			else if (key == "weaknesses")
			{ pokemon.weaknesses[0] = value; }
			else
			{
				fprintf(stderr, "Unsupported key: %s.\n", key.c_str());
				return false;
			}
		}
	}
	return false;
}

// Loads file into memory, then passes blocks separated by blank lines (\n\n) to load pokemon
bool
Pokedex::load_from_file
(const char* filename)
{
	// Read whole file into memory
	FILE* source_file = std::fopen(filename, "r");
	if (!source_file)
	{
		fputs("Error loading from file. File not open.", stderr);
		return false;
	}

	std::string source;
	{
		std::fseek(source_file, 0, SEEK_END);
		long size = std::ftell(source_file);
		std::fseek(source_file, 0, SEEK_SET);
		source.resize(size);
		std::fread(&source[0], 1, size, source_file);
	}
	std::fclose(source_file);

	// Look for pokemon block boundaries and pass to load_pokemon
	size_t pokemon_block_boundaries[2] = {0, 0};
	bool keep_looking_for_blocks = true;
	while(keep_looking_for_blocks)
	{
		// Look for next blank line
		pokemon_block_boundaries[0] = source.find_first_not_of("\n ", pokemon_block_boundaries[1]);
		pokemon_block_boundaries[1] = source.find("\n\n", pokemon_block_boundaries[0]);
		if (pokemon_block_boundaries[1] == std::string::npos)
		{
			// End of source file
			keep_looking_for_blocks = false;
			pokemon_block_boundaries[1] = source.length();
		}
		size_t pokemon_block_length = pokemon_block_boundaries[1] - pokemon_block_boundaries[0];
		load_pokemon( source.substr( pokemon_block_boundaries[0], pokemon_block_length ) );
	}
	return true;
}

void
Pokedex::sort
(Pokedex::SortType type)
{
	std::shared_ptr<PokedexPokemonEntry> current_undefined = first_pokemon_entry;
	while (current_undefined != nullptr)
	{
		std::shared_ptr<PokedexPokemonEntry> best_candidate = current_undefined;
		std::shared_ptr<PokedexPokemonEntry> current_candidate = current_undefined;
		while (current_candidate != nullptr)
		{
			if (type == SortType::ID  && current_candidate->pokemon.global_id < best_candidate->pokemon.global_id)
			{ best_candidate = current_candidate; }
			current_candidate = current_candidate->successor;
		}
		Pokemon tmp = current_undefined->pokemon;
		current_undefined->pokemon = best_candidate->pokemon;
		best_candidate->pokemon = tmp;
		current_undefined = current_undefined->successor;
	}
	return;
}