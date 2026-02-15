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
"Listing the entries in memory (can be used to write to a source file):"
"\n"
"\t--print"
"\n\n"
"Sorting, should appear a single time:"
"\n"
"\t--sort <size|stage>"
"\n\n"
"Adding an entry to pokedex. OBS: See representation syntax."
"\n"
"\t--add <pokemon representation>"
"\n\n"
"Deleting a pokemon by its name:"
"\n"
"\t--remove <pokemon name>"
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
		// --add <pokemon>
		else if (strcmp(key, "--add") == 0)
		{
			pokedex.load_pokemon(argv[i+1]);
		}
		// --remove <pokemon>
		else if (strcmp(key, "--remove") == 0)
		{
			if (pokedex.remove(argv[i+1]))
			{ printf("Removed %s from pokedex.\n.", argv[i+1]); }
			else { fprintf(stderr, "Pokemon %s not found.\n", argv[i+1]); }
		}
		// Source file
		// --source <file>
		else if (strcmp(key, "--source") == 0 && i == 1)
		{
			pokedex.load_from_file(argv[++i]);
		}
		else if (strcmp(key, "--print") == 0)
		{
			pokedex.print_to_stdout();
		}
		else if (strcmp(key, "--size") == 0)
		{
			std::cout << pokedex.size() << std::endl;
		}
		else if (strcmp(key, "--sort") == 0)
		{
			if (strcmp(argv[i+1], "id") == 0)
			{ pokedex.sort(Pokedex::SortType::ID); }
			else
			{
				perror("Unknown sorting type.\n");
				return ParsingResult::ERROR;
			}
		}
		else if (strcmp(key, "--show") == 0)
		{
			Pokemon pokemon = pokedex.get_by_name(argv[i+1]);
			if (pokemon.name == "")
			{ perror("Pokemon not found.\n"); return ParsingResult::ERROR; }
			else
			{ pokemon.print(); }
		}
	}
	return ParsingResult::CONTINUE;
}