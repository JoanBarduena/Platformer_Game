# Upside Down
In this repository there is a platformer video game made by two second year students of the Videogame Design and Development degree.
The game has been made using C++ (using SDL and pugi libraires) and the maps have been created using Tiled.

 ## How to play
The game is quite simple. You must reach the end of each level (there are two) collecting all the coins and avoiding enemies. You start with three lives but you can collect 1 life in every level. Game controls:

~~~

**A** - Move player left.

**D** - Move player right.

**W** - Move player up (only with God Mode).

**S** - Move player down (only with God Mode).

**SPACE** - Jump.

**L** - Invert Gravity.

~~~

## Download Game
To download the last release of UpsideDown click [here](https://github.com/JosepLleal/Platformer_Game/releases/download/1.0/Upside_Down.zip).

## Authors:

![](team_photo.jpeg)

- Josep Lleal Sirvent
	-[Josep's GitHub Link](https://github.com/JosepLleal)
- Joan Barduena Reyes
	-[Joan's GitHub Link](https://github.com/JoanBarduena)

## Main core subsystems of the game

![](App.jpg)

 ## Gameplay
 
 <iframe width="560" height="315" src="https://www.youtube.com/embed/ToQxuUxstRU" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

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

## License

~~~

MIT License

Copyright (c) 2017 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

~~~
