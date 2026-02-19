#include <cstdio>
#include <cstring>
#include "graphical/pokedex.hpp"

int
main
(
	int argc,
	char **argv
)
{
	GraphicalPokedex graphical_pokedex;
	for(int i = 1; i < argc; i++)
	{
		char *key = argv[i]; // value = argv[i+1]
		if (strcmp(key, "--font") == 0)
		{
			graphical_pokedex.font_pattern_string = argv[i+1];
		}
	}
	if (graphical_pokedex.run())
	{ return EXIT_SUCCESS; }
	else { return EXIT_FAILURE; }
}