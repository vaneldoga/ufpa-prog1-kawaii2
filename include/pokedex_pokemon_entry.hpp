#include <memory>
#include "pokemon.hpp"

class PokedexPokemonEntry
{
public:
	std::weak_ptr<PokedexPokemonEntry> predecessor;
	std::shared_ptr<PokedexPokemonEntry> successor;
	Pokemon pokemon;
};