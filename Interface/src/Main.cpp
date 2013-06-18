#include "Main.h"

TileGrid grid;
Background bg;
Hud hud;

UIConfig cfg;

float currentTime = 0;
int main(int argc, char * argv[])
{  
	loadUIProperties(&cfg);
	window::setGrid(&grid);
	window::init(argc, argv, &cfg);

	grid.init(&cfg);
	grid.initGLComponents();

	hud.init(.5, .5, 1, 1, &cfg);
	window::setHUD(&hud);
	
	if(cfg.ext_tile_rep)printf("\nREPORTING TILES.\n");
	loadTiles(&grid, &cfg, cfg.paths.at(0));

	bg.init(&grid, &cfg); 
	
	input::init(&grid, &hud, &bg, &cfg);

	window::setBG(&bg);
	printf("A:%s B:%s F:%s S:%s W:%s\n",  &cfg.sdk_a, &cfg.sdk_b, &cfg.sdk_f, &cfg.select, &cfg.window);
	printf("LEFT: %d\n", GLUT_KEY_LEFT);
	printf("X: %d, Y: %d \n", cfg.x_res, cfg.y_res);
	printf("Current Tile: %s\n", grid.getCurrent()->getName());

	glutMainLoop();
	printf("\nPress Enter to exit...");
	int x = getchar();
}