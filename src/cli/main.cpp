#include <cstdio>
#include <cstring>

#include "pokedex.hpp"

struct CommandLineParseResult
{
	bool success = false;
	const char* source_file = NULL;
	const char* output_file = NULL;
};

// Command line arguments parsing
struct CommandLineParseResult
command_line_parse
(
	Pokedex &pokedex,
	int argc,
	char **argv
);

// Definitions

struct CommandLineParseResult
command_line_parse
(
	Pokedex &pokedex,
	int argc,
	char **argv
)
{
	(void)pokedex; // TODO: use this
	struct CommandLineParseResult parse_result = { .success = false };

	if (argc <= 1)
	{ return parse_result; }

	// Look for keys and delegate their values to specific functions
	for(int i = 1; i < argc; i++)
	{
		char *key = argv[i]; // value = argv[++i]
		// --add <pokemon>
		if (strcmp(key, "--add") == 0)
		{
			printf("Adding by parsing: %s\n.", argv[++i]);
			parse_result.success = true;
		}
		// Source file
		// --source <file>
		else if (strcmp(key, "--source") == 0)
		{
			parse_result.source_file = argv[++i];
			parse_result.success = true;
		}
	}
	return parse_result;
}

int
main
(
	int argc,
	char **argv
)
{
	Pokedex pokedex;
	struct CommandLineParseResult parse_result = command_line_parse(pokedex, argc, argv);
	if (!parse_result.success)
	{
		fprintf(stderr, "Error parsing commandline.\n");
		return EXIT_FAILURE;
	}
	pokedex.load_from_file(parse_result.source_file);
	printf("Pokedemons in the pokedex: %d.\n", pokedex.size());
	return EXIT_SUCCESS;
}