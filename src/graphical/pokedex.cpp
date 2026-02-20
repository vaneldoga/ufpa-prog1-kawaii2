#include "graphical/pokedex.hpp"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <cstring>
#include <iostream>
#include <fontconfig/fontconfig.h>

bool
GraphicalPokedex::start
(void)
{
	memset(source_filename, 0, sizeof(source_filename));
	enum { CONTINUE, OPEN, QUIT } state;
	state = CONTINUE;
	while (state == CONTINUE)
	{
		if (WindowShouldClose())
		{ state = QUIT; }
		else
		{
			BeginDrawing();
			ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
			GuiLabel({ 270, 185, 260, 20 }, "Enter filename below:");
			GuiTextBox({ 270, 210, 260, 30 }, source_filename, 256, true);
			if (GuiButton({ 350, 260, 100, 30 }, "Open") || IsKeyPressed(KEY_ENTER))
			{
				TraceLog(LOG_INFO, "Attempting to open: %s", source_filename);
				state = OPEN;
			}
			EndDrawing();
		}
	}
	if (state == QUIT)
	{ return false; }
	if (!pokedex.load_from_file(source_filename))
	{ perror("Couldn't load source file.\n"); return false; }
	unsaved_changes = false;
	//std::cout << pokedex.at(0).name << std::endl;
	return true;
}

GraphicalPokedex::State
GraphicalPokedex::view
(void)
{
	int current_position = 0;
	// Appears at top left corner. Ex: 2/35.
	// 24 is probably enough.
	char position_indicator[24];
	memset(position_indicator, 0, sizeof(position_indicator));
	while (!WindowShouldClose())
	{
		Pokemon current_pokemon = pokedex.at(current_position);
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
		snprintf(position_indicator, sizeof(position_indicator), "%d/%d", current_position+1, pokedex.get_size());
		GuiLabel({ 40, 40, 50, 20 }, position_indicator);
		GuiLabel({ 270, 180, 260, 20 }, current_pokemon.name.c_str());
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(SKYBLUE));
		bool is_button_pressed_previous = GuiButton({ 100, 600, 200, 30 }, "<-- Previous");
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(GREEN));
		bool is_button_pressed_add = GuiButton({ 300, 600, 100, 30 }, "Add (A)");
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(GetColor(0xff4f4fff)));
		bool is_button_pressed_delete = GuiButton({ 400, 600, 100, 30 }, "Delete (D)");
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(SKYBLUE));
		bool is_button_pressed_next = GuiButton({ 500, 600, 200, 30 }, "Next -->");
		if (IsKeyPressed(KEY_D) || is_button_pressed_delete)
		{ pokedex.remove(current_pokemon.name); }
		else if (
			(is_button_pressed_previous || IsKeyPressed(KEY_LEFT))
			&& current_position > 0
		)
		{ current_position--; }
		else if
		(
			(is_button_pressed_next || IsKeyPressed(KEY_RIGHT))
			&& current_position < pokedex.get_size()-1
		)
		{ current_position++; }
		EndDrawing();
	}
	return GraphicalPokedex::State::START;
}

void
GraphicalPokedex::load_font
(void)
{
	if (!FcInit())
	{ perror("Couldn't start FontConfig.\n"); return; }
	if (font_pattern_string == NULL)
	{ font_pattern_string = "Inconsolata:style=Bold"; }
	FcPattern *pattern = FcNameParse((const FcChar8*)font_pattern_string);
	FcConfigSubstitute(NULL, pattern, FcMatchPattern);
	FcDefaultSubstitute(pattern);
	FcResult result;
	FcPattern* match = FcFontMatch(NULL, pattern, &result);
	// If found, use it, otherwise keep default.
	if (match)
	{
		FcChar8* file = NULL;
		if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch)
		{ printf("Found file: %s\n", file); }
		else
		{ perror("Font found, but no path property\n."); }
		std::cout << (char*)file << std::endl;
		Font pokedex_font = LoadFontEx((char*)file, 18, 0, 0);
		GuiSetFont(pokedex_font);
		GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
	}
	FcPatternDestroy(match);
	FcPatternDestroy(pattern);
	FcFini();
}

bool
GraphicalPokedex::run
(void)
{
	InitWindow(800, 800, "GUI Pokedex");
	SetTargetFPS(30);

	bool should_close = false;
	GraphicalPokedex::State state = GraphicalPokedex::State::START;
	load_font();

	while (!should_close)
	{
		switch (state)
		{
			case GraphicalPokedex::State::START:
				if (!start())
				{ should_close = true; }
				else
				{ state = GraphicalPokedex::State::VIEW; }
				break;
			case GraphicalPokedex::State::VIEW:
			case GraphicalPokedex::State::ADD:
			case GraphicalPokedex::State::DELETE:
			case GraphicalPokedex::State::EDIT:
				state = view();
				break;
		}
	}
	CloseWindow();
	return true;
}