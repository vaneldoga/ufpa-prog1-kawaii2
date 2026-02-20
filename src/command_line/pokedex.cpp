#include <cstring>
#include <cstdio>
#include <iostream>
#include "command_line/pokedex.hpp"

const char *CommandLinePokedex::help_message =
"\n"
"# Pokedex CLI"
"\n\n"
"Non-interactive mode for a pokedex."
"\n\n"
"It stores pokemon entries with a few stats, can parse a file or print to one and perform operations like adding, edditing, removing and sorting entries by command line flags."
"\n\n"
"# Flags"
"\n\n"
"Load entries into memory from a source file, should appear as first flag:"
"\n"
"\t--source <file>"
"\n\n"
"Getting how many entries in memory:"
"\n"
"\t--size"
"\n\n"
"Listing the entries in memory (can be used to write to a source file) or a previously selected pokemon (see --select):"
"\n"
"\t--print"
"\n\n"
"Sorting, should appear a single time:"
"\n"
"\t--sort <size|stage>"
"\n\n"
"Adding an entry to pokedex. OBS: See representation syntax."
"\n"
"\t--add <representation>"
"\n\n"
"# Selection"
"\n\n"
"Remove and edit operations need a previously selected pokemon."
"\n"
"\t--select <name>"
"\n\n"
"Deleting a selected pokemon:"
"\n"
"\t--remove"
"\n\n"
"Editting a selected pokemon:"
"\n"
"\t--set-<id|stage|weight|name> <value>"
"\n\n"
"Unselecting (useful to prevent --print from printing a single entry after a update):"
"\n"
"\t--unselect"
"\n\n"
"# Examples"
"\n\n"
"Add an entry and save it to a file:"
"\n"
"\tpokedex-cli --add 'id: 12; name: pikachu;' --print >> test.txt"
"\n\n"
"Load a file into memory, add an entry and save the modified pokedex to another file:"
"\n"
"\tpokedex-cli --source test-in.txt --add 'id: 12; name: pikachu;' --print >> test-out.txt"
"\n\n"
"Updating:"
"\n"
"\tpokedex-cli --source test/littledex.txt --select machomp --set-weight 2.4 --unselect --print"
"\n\n"
"Sorting:"
"\n"
"\tpokedex-cli --source test/littledex.txt --sort id --print"
"\n\n"
":3"
"\n\n"
;

CommandLinePokedex::ParsingResult
CommandLinePokedex::parse
(
	int argc,
	char **argv
)
{
	if (argc <= 1)
	{
		return ParsingResult::ERROR;
	}

	std::string selected_name = "";
	// Look for keys and delegate their values to specific functions
	for(int i = 1; i < argc; i++)
	{
		char *key = argv[i]; // value = argv[++i]
		// Help
		if (strcmp(key, "--help") == 0)
		{
			printf("%s", help_message);
			return ParsingResult::GRACEFUL_EXIT;
		}
		// Source file
		// --source <file>
		else if (strcmp(key, "--source") == 0 && i == 1)
		{
			pokedex.load_from_file(argv[++i]);
		}
		// --add <pokemon>
		else if (strcmp(key, "--add") == 0)
		{
			pokedex.load_pokemon(argv[i+1]);
		}
		// --select <name>
		else if (strcmp(key, "--select") == 0)
		{ selected_name = argv[i+1]; }
		// --unselect
		else if (strcmp(key, "--unselect") == 0)
		{ selected_name = ""; }
		// --remove
		else if (strcmp(key, "--remove") == 0)
		{
			if (selected_name.empty())
			{ perror("Error. No pokemon selected.\n"); return ParsingResult::ERROR; }
			else if (pokedex.remove(selected_name))
			{ printf("Removed %s from pokedex.\n.", argv[i+1]); selected_name = ""; }
			else { fprintf(stderr, "Error. Pokemon %s not found.\n", argv[i+1]); }
		}
		// --set-<field> <value>
		else if (strncmp(key, "--set-", strlen("--set-")) == 0)
		{
			// Fetch
			Pokemon pokemon = pokedex.get(selected_name);
			// Change
			const char* field = key+strlen("--set-"); // Skip --set-.
			static const char* field_id = "id";
			static const char* field_name = "name";
			static const char* field_evolution_stage = "stage";
			static const char* field_weight = "weight";
			if (strncmp(field, field_id, strlen(field_id)) == 0)
			{ pokemon.global_id = std::stoi(argv[i+1]); }
			else if (strncmp(field, field_evolution_stage, strlen(field_evolution_stage)) == 0)
			{ pokemon.evolution_stage = std::stoi(argv[i+1]); }
			else if (strncmp(field, field_weight, strlen(field_weight)) == 0)
			{ pokemon.weight = std::atof(argv[i+1]); }
			// TODO: bug here, name isn't set.
			else if (strncmp(field, field_name, strlen(field_name)) == 0)
			{ pokemon.name = argv[i]; }
			else
			{
				fprintf(stderr, "Error. Unknown field to set: %s.\n", field);
				return ParsingResult::ERROR;
			}
			// Update
			pokedex.update(selected_name, pokemon);
		}
		// --print
		// Pokemon (if selected) or entire pokedex
		else if (strcmp(key, "--print") == 0)
		{
			if (selected_name.empty())
			{ pokedex.print(); }
			else
			{
				Pokemon pokemon = pokedex.get(selected_name);
				pokemon.print();
			}
		}
		else if (strcmp(key, "--size") == 0)
		{
			std::cout << pokedex.get_size() << std::endl;
		}
		else if (strcmp(key, "--sort") == 0)
		{
			if (strcmp(argv[i+1], "id") == 0)
			{ pokedex.sort(Pokedex::SortType::ID); }
			else if (strcmp(argv[i+1], "weight") == 0)
			{ pokedex.sort(Pokedex::SortType::WEIGHT); }
			else
			{
				perror("Unknown sorting type.\n");
				return ParsingResult::ERROR;
			}
		}
		// We know it is a flag (or should be), but don't recognize it.
		else if (key[0] == '-')
		{
			fprintf(stderr, "Unknown flag: %s.\n", key);
			return ParsingResult::ERROR;
		}
	}
	return ParsingResult::CONTINUE;
}