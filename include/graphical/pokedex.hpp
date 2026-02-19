#include "../pokedex.hpp"

class GraphicalPokedex
{
public:
	// Main state of the program.
	enum class State
	{
		START,
		VIEW,
		ADD,
		DELETE,
		EDIT,
	};

	// Runs the main loop. Returns false if error occurred.
	bool
	run
	(void);

	const char* font_pattern_string = NULL;

protected:
	Pokedex pokedex;
	char source_filename[256];
	bool unsaved_changes;

	// Lets you traverse the pokedex and view pokemons.
	State
	view
	(void);

	// Start menu, for you to choose a source file or start from zero.
	// Returns true to go to State START, or false to quit.
	bool
	start
	(void);

	// Helper for run
	void
	load_font
	(void);
};