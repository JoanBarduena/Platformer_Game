# Upside Down
In this repository there is a platformer video game made by two second year students of the Videogame Design and Development degree.
The game has been made using C++ (using SDL and pugi libraires) and the maps have been created using Tiled.

## Controls:
**A** - Move player left.

**D** - Move player right.

**W** - Move player up (only with God Mode).

**S** - Move player down (only with God Mode).

**SPACE** - Jump.

**F** - Invert Gravity.

## Debug Keys:
**F1** - Start from the very first level.

**F2** - Start from the beginning of the current level.

**F3** - Start from the second level.

**F5** - Save the current state.

**F6** - Load the previous state (even across levels).

**F9** - To view colliders and pathfinding.

**F10** - God Mode (allows to fly around).

**F11** - Enable/Disable FPS cap to 30.

## Github repository:
https://github.com/JosepLleal/Platformer_Game

## Authors:
- Josep Lleal Sirvent
	-[Josep's GitHub Link](https://github.com/JosepLleal)
- Joan Barduena Reyes
	-[Joan's GitHub Link](https://github.com/JoanBarduena)

## Innovation:
- Level_load: Instead of creating 2 scenes, we develop a Stuct Level with an Int (level) and a Sting (MapPath.tmx). So if the int is "1" the map loaded will be "map1.tmx". 

## Tasklist: 
- Josep Lleal:
	- Pathfinding.
	- Entities (smasher, bat).
	- UML File. 

- Joan Barduena:
	- God Mode.
	- Save and load.
	- Sounds.
	- Map improvements.
	- Animations. 

- Both members:
	- Brofiler. 
	- Dt and Framerate Cap. 
	- Solving memory leaks.
	- Solving buggs. 
	- Collisions. 
	- Entities(player). 

## Sprites: 
Keyboard VirtualKeys: https://hyohnoo.itch.io/keyboard-controller-keys

Character: https://rvros.itch.io/animated-pixel-hero

Tileset: https://opengameart.org/content/platformer-tilesets

Enemy(bat): https://opengameart.org/content/bat-sprite
Enemy(smasher): https://www.spriters-resource.com/ds_dsi/henryhats/sheet/41281/?source=genre
Coins: https://donhellsing.itch.io/coin-animation
Hearts: https://donhellsing.itch.io/heart1

## Music: 
Name: "Galway" 

By: Kevin MacLeod (incompetech.com)

Licensed under Creative Commons: By Attribution 3.0 License

This game has been created by Josep LLeal and Joan Barduena. 
Licensed under the [MIT License](LICENSE).
