** simple dummy pokedéx were you can list your favorite pokemons :3**

Pokemon entry structure:

 - int: global pokedex index (`global`)

 - string: name

 - string: ability

 - int: evolution stage (`evolution`)

 - float: weight

 - int: generation

 - vetor de string: weaknesses

 - vetor de string: strengths

 - vetor de string: immunities

 - vetor de string: egg group (`breeding`)

Entries are persisted in a text database, with key value pairs, each on a line. A blank line separates the pokedéx entries.

Example:

	global: 45
	name: vileplume
	evolution state: 3
	weight: 18.6
	breeding: grass