ArcadeInterface
===============

A graphical user interface that allows a user to easily select a game on a multi-cade.

Features
========
* Tile based interface for easy and intuitive navigation.
* Multiple Tile layers to allow for multiple user-visible tile arrangements and easy sorting.
* Each tile can be extensively customized.
* Low memory requirements(depending on tile images)
* No platform-binding dependencies.
* Smooth navigation and simple effects.
* Uses OpenGL 2.0 to support a wide range of graphics hardware.

Tile Customization
=============
<b>An editor is included to construct tile layouts.<b> 
Each Tile has the following properties that can be modified.
* Location, highlighted and un-highlighted sizes.
* Whether or not it has a standard frame, is the first tile selected on startup, or if can be selected by the
user.
* Three seperate images are used to represent each tile--one that is displayed on the tile when it is 
highlighted, one that is displayed in the background when the tile is highlighted, and one that is displayed
on the tile when it is not highlighted.
* All tiles have a game name and description. (Eh, but these are as-of-now useless.)
* Each tile has four binding points that coorespond to the four cardinal directions which can be linked to any
other or no tile.

Interface Customization
=======================
The interface has many properties that can be configured as well.
* Control mappings.
* Sound effects.
* Universal texture images, such as the UI overlay, tile frame, and exit warning images.
* Transition and shutdown times.
* Fullscreen rendering.
* Window title, resolution and position.
* The ability to display extended information about the tile loading process and shader linking and compiling.
* Shader source code.
* Tile configuration attribute names can be configured. (For teh lulz)
* Even window blanking color can be personalized.

Building from Source
====================
By creating your own Visual C++ project using this source code this repository loses about 60MB in baby fat.
* Dump the source into a new, empty VC++ project.
* Set up the Include and Library directories in the VC++ Directories property page to look at the lib, GL, 
and bin directories.
* List out all the *.lib files in the dependencies/lib directory in the Linker->Input->Additional Dependencies 
property page.
* Wrestle with all the crap GLEW gives everyone on their first try.
* Do what you want with the assets to get the .exe to see them. I just leave them in the source directory 
until I need to distribute.

Running the Included Build
==========================
* Copy the assets/ directory into the same folder as the .exe
* Get any other .dlls it whines about.
* Run! Woohoo!
While the program may run, you need to configure the command of each Tile to do what you want. 
I doubt that you ahre going to have your roms in the samde directory as mine, for example.
