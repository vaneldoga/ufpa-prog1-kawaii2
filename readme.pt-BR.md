**Uma pokedex simplificada e personalizável, onde você pode listar seus pokemons favoritos :3**

Estrutura dos dados dos pokemons:

 - int: global pokedex index (`global_id` | usado pra identificação do pokemon. É atribuído automaticamente de forma sequencial.

 - string: name | nome do pokemon.

 - string list: abilities | habilidades do pokemon, variando entre 1 e 3, mas podendo aumentar em casos específicos como o de pokemons com mega-evolução(não implementado)

 - int: base stat total (`base_stat_total`) | soma de todos os atributos base de um pokemon. (atributos base inviduais não implementados.)

 - float: weight | peso de um pokemon.

 - int: generation | geração em que o pokemon foi introduzido. Normalmente varia entre 1 e 9, mas é permitido outros valores para pokemons personalizados.

 - string list: weaknesses | Elementos aos quais o pokemon em questão é fraco.

 - string list: resistances | Elementos aos quais o pokemon em questão é resistente.

 - string list: immunities | Elementos aos quais o pokemon em questão é imune.

 - string list: egg group | Grupo biológico onde o pokemon pertence. Define com que outros pokemons ele pode cruzar(não implementado)

Os dados são mantidos em um arquivo .txt, pareados com uma chave para identificação. Uma linha vazia separa os pokemons. 

Example:

```
id: 45;
name: Vileplume;
abilities: Chlorophyll, Effect Spore;
base stat total: 490;
weight: 18.6;
generation: 1;
weaknesses: Fire, Ice, Flying, Psychic;
resistances: Water, Grass, Electric, Fighting, Fairy;
immunities: none;
egg group: Grass;
```

**Aspects:**

 - Padrão C++23.

 - estilo de IO em C (cstdio library) ao invés do de C++ (iostream) in many cases, proeminently for simple printing.

 - linked list feita à mão ao invés de std::vector.

**A se fazer:**

 - Interface gráfica opcional utilizando (Raylib).

## Building

**Com meson:**

```sh
meson setup build
ninja -C build
```

**Sem meson:**

```sh
g++ -I include/ src/command_line/main.cpp src/command_line/pokedex.cpp  src/pokemon.cpp src/pokedex.cpp -o pokedex-cli
``` 
ou
```sh
clang++ -I include/ src/command_line/main.cpp src/command_line/pokedex.cpp  src/pokemon.cpp src/pokedex.cpp -o pokedex-cli
``` 

## Testes
Testes são para ser feitos com o arquivo `test/dex.txt`, que contém os dados de todos os 1025 pokemons.

Inicialmente, os testes foram feitos com o arquivo `test/smalldex.txt`, mas ele agora está obsoleto, assim como o `bigdex.txt`, que só persistem para fins de análise. `bigdex.txt` foi gerado puxando um arquivo json de  [olitreadwell](https://github.com/olitreadwell/pokemon-classic-json/blob/main/pokedex.json), que reune 151 pokemons clássicos.
## License

Não-licenciado (domínio público)
