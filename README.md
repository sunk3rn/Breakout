# 🧱 Breakout
Projekt do předmětu UPR na téma Breakout, s pomocí knihovny SDL.

## 🕹️ Popis hry:
Breakout je arkádová videohra vyvinutá a vydaná společností Atari, Inc. a vydaná 13. května 1976. Navrhl ji Steve Wozniak na základě konceptu Nolana Bushnella a Steva Bristowa, kteří byli ovlivněni klíčovou arkádovou hrou Atari Pong z roku 1972. Ve hře Breakout je cílem zničit všechny cihly v horní části obrazovky opakovaným odrážením míče od pálky.

## 📕 Pravidla
Hra začíná s osmi řadami cihel, po dvou řadách jiné barvy. Pořadí barev zdola nahoru je žlutá, zelená, oranžová a červená. Pomocí jedné koule musí hráč srazit co nejvíce cihel pomocí zdí a/nebo pádla pod nimi, aby zasáhl míč o cihly a zlikvidoval je. Pokud hráčova pálka mine odskok míče, ztratí život. Hráč má tři životy na to, aby vyčistit obrazovku od cihel. Žluté cihly získávají po jednom bodu, zelené cihly tři body, oranžové cihly pět bodů a červené cihly nejvyšší úrovně po sedmi bodech. Pálka se zmenší na polovinu své velikosti poté, co míč prorazí červenou řadu a narazí na horní stěnu. Rychlost míče se zvyšuje v určitých intervalech: po čtyřech úderech, po dvanácti úderech a po kontaktu s oranžovou a červenou řadou. 

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
