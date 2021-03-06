#include "Main.h"

// The application's TileGrid instance.
TileGrid grid;

// The application's Background instance.
Background bg;

// The application's Hud/Overlay instance.
Hud hud;

// The config struct used to store data used to initialize everything.
UIConfig cfg;

int main(int argc, char * argv[])
{  
	// Load the configuration preferences from a file into the config struct.
	loadUIProperties(&cfg);

	// Initialize the sound engine.
	audio::init(&cfg);
	
	// Initialize the window.
	window::init(argc, argv, &cfg);

	printf("==INITIALIZING APPLICATION COMPONENTS==========================================\n");

	// Initialize the TileGrid.
	grid.init(&cfg);

	// Initialize the OpenGL components of the TileGrid.
	grid.initGLComponents();

	// Initialize our Hud instance.
	hud.init(.5, .5, 1, 1, &cfg);

	// Initialize the background.
	bg.init(&grid, &cfg); 

	printf("===============================================================================\n");
	printf("Done initializing components.\n\n");

	// Assign our TileGrid instance to the window.
	window::setGrid(&grid);

	// Assign the HUD to the window.
	window::setHUD(&hud);
	
	// Load the Tile properties from file.
	loadTiles(&grid, &cfg);
	
	// Initialize the input code.
	input::init(&grid, &hud, &bg, &cfg);

	// Assign our background instance to the window.
	window::setBG(&bg);

	// Start the BGM!
	audio::startBGM();

	// Start the GLUT rendering loop. No going back now, sweetheart.
	glutMainLoop();

	// Just in case something dramatic happens we have an exit confirmation.
	printf("\nPress Enter to exit...");
	int x = getchar();
}