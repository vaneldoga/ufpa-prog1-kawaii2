#include <cstdio>
#include <cstring>
#include <iostream>

#include "command_line/pokedex.hpp"

#define MENU_OPTION_1 "um"
#define MENU_OPTION_2 "dois"
#define MENU_OPTION_3 "tres"
#define MENU_OPTION_4 "quatro"
#define MENU_OPTION_5 "cinco"
#define MENU_OPTION_6 "seis"

int
userinput
(void)
{
    int option = 0;
    std::string option_string;
    bool display_error_message = false;
    
    while (!(option >= 1 && option <= 6))
    {
        if (display_error_message)
            printf("Error: Invalid User Input. Please try again.\n");
        
        if (std::cin >> option)
        {
            if (option >= 1 && option <= 6)
                return option;
        }
        else std::cin.clear(); 
        
        if(std::cin >> option_string)
        {
                if (option_string == MENU_OPTION_1)
                    option = 1;
                else if (option_string == MENU_OPTION_2)
                    option = 2;
                else if (option_string == MENU_OPTION_3)
                    option = 3;
                else if (option_string == MENU_OPTION_4)
                    option = 4;
                else if (option_string == MENU_OPTION_5)
                    option = 5;
                else if (option_string == MENU_OPTION_6)
                    option = 6;
        }
        display_error_message = true;
    }
    return option;       
}

int
main
(
    int argc,
    char **argv
)
{
    if (argc <= 1)
    {
        Pokedex pokedex;
        bool running = true;

        std::cout << "====== Pokedex TUI ======\n";

        while (running)
        {
            std::cout << "\n======================\n";
            std::cout << "1. Load file\n";
            std::cout << "2. Add Pokemon\n";
            std::cout << "3. Search Pokemon\n";
            std::cout << "4. Print Pokemon\n";
            std::cout << "5. Remove Pokemon\n";
            std::cout << "6. Save and Exit\n";
            std::cout << "========================\n";
            printf("Select an operation.\n");
            
            int option = userinput();

            switch (option)
            {
                case 1:
                {
                    std::string filename;
                    std::cout << "Type the file name: ";
                    std::cin >> filename;
                    if (pokedex.load_from_file(filename.c_str()))
                    { std::cout << "File loaded successfully\n"; }
                    break;
                }
                

		case 2:
		{
		    Pokemon p;
	
		    int current_size = pokedex.get_size();
		    if (current_size == 0)
		    { p.global_id = 1; }
		    else
		    { p.global_id = pokedex.at(current_size - 1).global_id + 1; }
			
		    std::cout << "ID: " << p.global_id << "\n";
			
		    std::cout << "Name: ";
		    std::cin >> std::ws;
		    std::getline(std::cin, p.name);
			
		    std::cout << "Base stat total: ";
		    std::cin >> p.base_stat_total;
			
		    std::cout << "Weight: ";
		    std::cin >> p.weight;
			
		    std::cout << "Generation: ";
		    std::cin >> p.generation;
			
		    std::cout << "Abilities: ";
		    std::cin >> std::ws;
		    std::getline(std::cin, p.abilities[0]);
			
		    std::cout << "Weaknesses: ";
		    std::cin >> std::ws;
		    std::getline(std::cin, p.weaknesses[0]);
			
		    std::cout << "Resistances: ";
		    std::cin >> std::ws;
		    std::getline(std::cin, p.resistances[0]);
			
		    std::cout << "Immunities: ";
		    std::cin >> std::ws;
		    std::getline(std::cin, p.immunities[0]);
			
		    std::cout << "Egg group: ";
	            std::cin >> std::ws;
		    std::getline(std::cin, p.egg_group[0]);
		
		    if (pokedex.add(p))
		    { std::cout << "Pokemon added to the end of the Pokedex.\n"; }
		    break;
		    }

		case 3:
                {
                    std::cout << "Search by:\n1. Name\n2. ID\nChoose: ";
                    int search_opt;
                    std::cin >> search_opt;
                    if (search_opt == 1)
                    {
                        std::string name;
                        std::cout << "Type the name: ";
                        std::cin >> name;
                        Pokemon p = pokedex.get(name);
                        if (p.name != "") 
			{ p.print(); }
                        else 
			{ std::cout << "Pokemon not found.\n"; }
                    }
                    else if (search_opt == 2)
                    {
                        unsigned int id;
                        std::cout << "Type the ID: ";
                        std::cin >> id;
                        Pokemon p = pokedex.get_by_id(id);
                        if (p.name != "") { p.print(); }
                        else { std::cout << "Pokemon not found.\n"; }
                    }
                    break;
                }
                case 4:
                {
                    std::cout << "1. Print All\n2. Print Interval\nChoose: ";
                    int print_opt;
                    std::cin >> print_opt;
                    if (print_opt == 1)
                    { pokedex.print(); }
                    else if (print_opt == 2)
                    {
                        int start, end;
                        std::cout << "First position: ";
                        std::cin >> start;
                        std::cout << "Last position: ";
                        std::cin >> end;
                        pokedex.print_range(start, end);
                    }
                    break;
                }
                case 5:
                {
                    std::string name;
                    std::cout << "Type the name to remove: ";
                    std::cin >> name;
                    if (pokedex.remove(name))
                    { std::cout << "Pokemon successfully removed.\n"; }
                    else
                    { std::cout << "Pokemon not found.\n"; }
                    break;
                }
                case 6:
                {
                    std::string filename;
                    std::cout << "File name to save the changes: ";
                    std::cin >> filename;
                    if (pokedex.save_to_file(filename.c_str()))
                    {
                        std::cout << "Data successfully recorded.\n";
                        running = false;
                    }
                    break;
                }
            }
        }
        return EXIT_SUCCESS;
    }

    CommandLinePokedex command_line_pokedex;
    CommandLinePokedex::ParsingResult parsing_result = command_line_pokedex.parse(argc, argv);
    switch(parsing_result)
    {
        case CommandLinePokedex::ParsingResult::ERROR:
            fprintf(stderr, "Error parsing commandline. Pass --help for usage.\n");
            return EXIT_FAILURE;
            break;
        case CommandLinePokedex::ParsingResult::GRACEFUL_EXIT:
            return EXIT_SUCCESS;
            break;
        case CommandLinePokedex::ParsingResult::CONTINUE:
            return EXIT_SUCCESS;
            break;
    }
    return EXIT_FAILURE;
}
