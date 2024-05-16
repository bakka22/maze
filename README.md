Maze Project

Maze is a semi 3D game where you try to escape the maze
while facing and killing enemies, I did this project alone
which was a big benefit for me because I learned so much
from it and I enjoyed coding every bit of it. being a simple
game I focused on graphics to get the ultimate experience 
from it.

project blog post: https://www.linkedin.com/posts/abobaker-eltom-3a1a90147_activity-7196348105389178881-eytZ?utm_source=share&utm_medium=member_desktop .
My linkedin Profile:  https://www.linkedin.com/in/abobaker-eltom-3a1a90147/ .

![wep1](https://github.com/bakka22/maze/assets/135711566/257628d2-c656-4924-b186-e295370b2e04)



* Installation:
	use gcc compiler with SDL flags to compile all .c files to make the executable.
	gcc *.c -o <executable_name> `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image 

* Usage:
	execute a the file providing the map file name as an argument:
./<executable_name> <map_file_name>
a map file called map is provided or you can create your own mapfile.
the syntax for a map file is:

first line:
player's x, y coordinates terminated by a semicolon.
for example:
53, 86;
-------
second line:
enemies x, y coordinates seperated with semicolons.
for example:
43, 59; 23, 243; 65, 23;
-------
third line:
first value represent the map's width, the second value represent the map's height.
for example:
20, 20;
-------
fourth to the last line:
represents the map.
use 1 to represent a wall and 0 for no wall.
a line represents a row which has width columns.
for example if the dimensions of the map is 6, 5 the map would look something like this:
1, 1, 1, 1, 1, 1;
1, 0, 0, 0, 0, 1;
1, 0, 0, 0, 0, 1;
1, 0, 0, 0, 0, 1;
1, 1, 1, 1, 1, 1;
**NOTE: it is very important that the dimensions provided are consistant
with the rows and columns of the map, otherwize the game might crash.

-------------------------------------------------

* Contribution:
abobaker altom babiker <abobakereltom@gmail.com> .




