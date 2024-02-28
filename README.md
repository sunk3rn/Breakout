## [EN version](ENREADME.md)
# 🧱 Breakout 
Projekt do předmětu UPR na téma Breakout, s pomocí knihovny SDL. (Pokud to ukradnete, tak vás kelvin dostane!)

## 📕 Pravidla
Pomocí jedné koule musí hráč srazit co nejvíce cihel pomocí zdí a/nebo pádla pod nimi, aby zasáhl míč o cihly a zlikvidoval je. Pokud hráčova pálka mine odskok míče, ztratí život. Hráč má tři životy na to, aby vyčistit obrazovku od cihel. 

# 🕹️ Popis hry:

## 🏗️ Instrukce pro přeložení

Pro překlad je potřeba:
- CMake
- SDL2
- SDL2-ttf

```bash
mkdir build
cd build
cmake ..
make && ./hra
```

## Před spuštěním
- Počáteční herní pole lze nastavit v souboru `starting-field.txt`, pokud bude tento soubor špatně formátován nebo nebude existovat, načte se základní konfigurace pole
- Validní formát je pole 15x15, každý blok může mít od 0 do 11 životů, životy se projeví jeho barvou. 0 = Mezera, prázdný blok
- Tři nejlepší skóre se ukládájí do souboru `score.txt` od největšího po nejmenší, pokud je skóre menší než nejmenší, tak se neuloží 

## 🎮 Jak hrát

### 🎛️ Ovládání
- V menu se lze pohybovat pomocí šipek, potvrzovat pomocí klávesy `Enter`. 
- Jakmile spustíte hru, tak pro vypuštění míčku zmáčkněte mezerník, pálkou lze hýbat buď myší a nebo šipkami (Podle nastavení)
### ⚙️ Nastavení
- Lze nastavit ovládání pomocí myši nebo klávesnice a počet životů při startu hry
  

## Problémy:
- Míček dělá binec při odrazu od boku bloků
- Odrazy míčku nejsou náhodné