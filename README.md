# 🧱 Breakout
Projekt do předmětu UPR na téma Breakout, s pomocí knihovny SDL.

## 🕹️ Popis hry:
Breakout je arkádová videohra vyvinutá a vydaná společností Atari, Inc. a vydaná 13. května 1976. Navrhl ji Steve Wozniak na základě konceptu Nolana Bushnella a Steva Bristowa, kteří byli ovlivněni klíčovou arkádovou hrou Atari Pong z roku 1972. Ve hře Breakout je cílem zničit všechny cihly v horní části obrazovky opakovaným odrážením míče od pálky.

## 📕 Pravidla
Pomocí jedné koule musí hráč srazit co nejvíce cihel pomocí zdí a/nebo pádla pod nimi, aby zasáhl míč o cihly a zlikvidoval je. Pokud hráčova pálka mine odskok míče, ztratí život. Hráč má tři životy na to, aby vyčistit obrazovku od cihel. 

## 🏗️ Build Instructions

Pro překlad je potřeba:
- CMake
- SDL2
- SDL2-ttf

---

```bash
mkdir build
cd build
cmake ..
make && ./hra
```
