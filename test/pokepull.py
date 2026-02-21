import requests
from bs4 import BeautifulSoup
import re
import time

def filltipagem(pkmn_types, weaknesses, resistances, immunities):
    tipos = [
        "Normal", "Fire", "Water", "Grass", "Electric", "Ice", "Fighting", 
        "Poison", "Ground", "Flying", "Psychic", "Bug", "Rock", "Ghost", 
        "Dragon", "Steel", "Dark", "Fairy"
    ]

    type_chart = { # isso aq pedi pro gemini preencher pq misericordia preencher isso aq na mao, a logica eh simples, so trabalhoso
        "Normal":   {"Rock": 0.5, "Ghost": 0.0, "Steel": 0.5},
        "Fire":     {"Fire": 0.5, "Water": 0.5, "Grass": 2.0, "Ice": 2.0, "Bug": 2.0, "Rock": 0.5, "Dragon": 0.5, "Steel": 2.0},
        "Water":    {"Fire": 2.0, "Water": 0.5, "Grass": 0.5, "Ground": 2.0, "Rock": 2.0, "Dragon": 0.5},
        "Electric": {"Water": 2.0, "Electric": 0.5, "Grass": 0.5, "Ground": 0.0, "Flying": 2.0, "Dragon": 0.5},
        "Grass":    {"Fire": 0.5, "Water": 2.0, "Grass": 0.5, "Poison": 0.5, "Ground": 2.0, "Flying": 0.5, "Bug": 0.5, "Rock": 2.0, "Dragon": 0.5, "Steel": 0.5},
        "Ice":      {"Fire": 0.5, "Water": 0.5, "Grass": 2.0, "Ice": 0.5, "Ground": 2.0, "Flying": 2.0, "Dragon": 2.0, "Steel": 0.5},
        "Fighting": {"Normal": 2.0, "Ice": 2.0, "Poison": 0.5, "Flying": 0.5, "Psychic": 0.5, "Bug": 0.5, "Rock": 2.0, "Ghost": 0.0, "Dark": 2.0, "Steel": 2.0, "Fairy": 0.5},
        "Poison":   {"Grass": 2.0, "Poison": 0.5, "Ground": 0.5, "Rock": 0.5, "Ghost": 0.5, "Steel": 0.0, "Fairy": 2.0},
        "Ground":   {"Fire": 2.0, "Electric": 2.0, "Grass": 0.5, "Poison": 2.0, "Flying": 0.0, "Bug": 0.5, "Rock": 2.0, "Steel": 2.0},
        "Flying":   {"Electric": 0.5, "Grass": 2.0, "Fighting": 2.0, "Bug": 2.0, "Rock": 0.5, "Steel": 0.5},
        "Psychic":  {"Fighting": 2.0, "Poison": 2.0, "Psychic": 0.5, "Dark": 0.0, "Steel": 0.5},
        "Bug":      {"Fire": 0.5, "Grass": 2.0, "Fighting": 0.5, "Poison": 0.5, "Flying": 0.5, "Psychic": 2.0, "Ghost": 0.5, "Dark": 2.0, "Steel": 0.5, "Fairy": 0.5},
        "Rock":     {"Fire": 2.0, "Ice": 2.0, "Fighting": 0.5, "Ground": 0.5, "Flying": 2.0, "Bug": 2.0, "Steel": 0.5},
        "Ghost":    {"Normal": 0.0, "Psychic": 2.0, "Ghost": 2.0, "Dark": 0.5},
        "Dragon":   {"Dragon": 2.0, "Steel": 0.5, "Fairy": 0.0},
        "Steel":    {"Fire": 0.5, "Water": 0.5, "Electric": 0.5, "Ice": 2.0, "Rock": 2.0, "Steel": 0.5, "Fairy": 2.0},
        "Dark":     {"Fighting": 0.5, "Psychic": 2.0, "Ghost": 2.0, "Dark": 0.5, "Fairy": 0.5},
        "Fairy":    {"Fire": 0.5, "Fighting": 2.0, "Poison": 0.5, "Dragon": 2.0, "Dark": 2.0, "Steel": 0.5}
    }

    for attacker in tipos:
        total_multiplier = 1.0
        for pkmn_type in pkmn_types:
            multiplier = type_chart.get(attacker, {}).get(pkmn_type, 1.0)
            total_multiplier *= multiplier

        if total_multiplier == 0:
            immunities.append(attacker)
        elif total_multiplier > 1:
            weaknesses.append(attacker)
        elif total_multiplier < 1:
            resistances.append(attacker)
    if len(immunities) == 0:
        imunidades.append("none")
    if len(resistances) == 0:
        resistances.append("none")

with open("dex.txt", 'w', encoding='utf-8') as file:
    for i in range(1, 1026):
        url = f"https://pokemondb.net/pokedex/{i}"
        r = requests.get(url)

        soup = BeautifulSoup(r.content, 'html.parser') #soup serve pra tornar o html algo q de pra varrer a procura dos dados
        
        pokename = soup.find('h1').text
        
        pokegen_raw = soup.find('abbr').text
        pokegen = re.findall(r'\d+', pokegen_raw)[0]

        types_list = [t.text for t in soup.select('th:-soup-contains("Type") + td a')]
        
        fraquezas, resistencias, imunidades = [], [], []
        filltipagem(types_list, fraquezas, resistencias, imunidades)

        abilities = list(dict.fromkeys([t.text for t in soup.select('th:-soup-contains("Abilities") + td a')]))
        
        weight_raw = soup.select_one('th:-soup-contains("Weight") + td').text
        weight_match = re.findall(r'(\d+\.\d+|\d+)\s?kg', weight_raw)
        pokepeso = weight_match[0] if weight_match else "Unknown"

        egg_groups = list(dict.fromkeys([t.text for t in soup.select('th:-soup-contains("Egg Groups") + td a')]))

        bst = soup.select_one('th:-soup-contains("Total") + td').text

        file.write(f"id: {i};\n")
        file.write(f"name: {pokename};\n")
        file.write(f"abilities: {', '.join(abilities)};\n")
        file.write(f"base stat total: {bst};\n")
        file.write(f"weight: {pokepeso};\n")
        file.write(f"generation: {pokegen};\n")
        file.write(f"weaknesses: {', '.join(fraquezas)};\n")
        file.write(f"resistances: {', '.join(resistencias)};\n")
        file.write(f"immunities: {', '.join(imunidades)};\n")
        file.write(f"egg group: {', '.join(egg_groups)};\n\n")
        
        print(f"{pokename} gerado;")
print("cabo")
