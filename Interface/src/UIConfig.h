#pragma once
#include <vector>
struct UIConfig
{
	bool ext_shader_rep;	// Whether or not to print extended shader creation debug info.
	bool ext_tile_rep;		// Whether or not to print extended Tile loading debug info.

	int x_res;				// The X resolution of the screen.
	int y_res;				// The Y resolution of the screen.

	int win_x;				// The X position of the window when not fullscreen.
	int win_y;				// The Y position of the window when not fullscreen, sans title-bar.

	bool fullscr;			// Whether or not to run the interface in full-screen mode.

	float clr_r;			// The R term of the screen-blanking color.
	float clr_g;			// The G term of the screen-blanking color.
	float clr_b;			// The B term of the screen-blanking color.

	char * title;			// The window Title. Only really displayed for one frame.

	char * frame_path;		// The file path of the frame texture, relative to the .exe.
	char * exit_path;		// The file path of the exit notification, relative to the .exe.
	char * ovr_path;		// The file path of the screen overlay, relative to the .exe.
	char * intro_path;		// The file path of the intro overlay, relative to the .exe.

	char * bg_v;			// The file path of the background's vertex shader.
	char * bg_f;			// The file path of the background's fragment shader.

	char * hud_v;			// The file path of the HUD's vertex shader.
	char * hud_f;			// The file path of the HUD's fragment shader.

	char * tile_v;			// The file path of the Tiles' vertex shader.
	char * tile_f;			// The file path of the Tiles' fragment shader.

	char * bgm;				// The filepath of the BGM.
	char * m_snd;			// The filepath of the movement sound.
	char * s_snd;			// The filepath of the selection sound.
	char * r_snd;			// The filepath of the rejected action sound.

	float vol;				// The overall sound volume.
	float bgm_vol;			// The volume of the BGM.
	float m_vol;			// The volume of the movement sound.
	float s_vol;			// The volume of the select sound.
	float r_vol;			// The volume of the rejected action sound.

	int anim_frames;		// The number of frames it takes to transition between Tiles.
	int shutdown_time;		// How long both shutdown keys must be pressed to exit the UI.
	bool play_intro;		// Whether or not to play the intro.
							
	char * nm;				// The name of the game/Tile nodes in the config files.
	char * ap;				// The name of the active_path attribute in each game/Tile node.
	char * ip;				// The name of the inactive_path attribute in each game/Tile node.
	char * bp; 				// The name of the bg_path attribute in each game/Tile node.
	char * n;				// The name of the name attribute in each game/Tile node.
	char * c;				// The name of the command attribute in each game/Tile node.
	char * d;				// The name of the description attribute in each game/Tile node.
	char * i;				// The name of the id attribute in each game/Tile node.
	char * ni;				// The name of the north_id attribute in each game/Tile node.
	char * si;				// The name of the south_id attribute in each game/Tile node.
	char * ei;				// The name of the east_id attribute in each game/Tile node.
	char * wi;				// The name of the west_id attribute in each game/Tile node.
	char * hf; 				// The name of the has_frame attribute in each game/Tile node.
	char * is;				// The name of the is_first attribute in each game/Tile node.
	char * x;				// The name of the x location attribute in each game/Tile node.
	char * y;				// The name of the y location attribute in each game/Tile node.
	char * sxa;				// The name of the sx_active attribute in each game/Tile node.
	char * sya;				// The name of the sy_active attribute in each game/Tile node.
	char * sxi;				// The name of the sx_inactive attribute in each game/Tile node.
	char * syi;				// The name of the sy_inactive attribute in each game/Tile node.
	char * s;				// The name of the selectablity attribute in each game/Tile node.
							
	std::vector<char *> paths;	// The container for all the the paths.
							
	int sdk_a;				// The first shutdown key.
	int sdk_b;				// The second shutdown key.
	int sdk_f;				// The fast shutdown key.
	int select;				// The Tile selection key.
	int window;				// The window/maximisation key.
	int n_cfg;				// The next-config button.
	int p_cfg;				// The prev-config button.
};

/*
Prints the contents of a config struct in a concise manner.
*/
void inline printConfig(UIConfig * c)
{
	printf("Window:\n");
	printf("Resolution: X=%4d Y=%4d\n", c->x_res, c->y_res);
	printf("Location: X=%4d Y=%4d\n", c->win_x, c->win_y);
	printf("Run fullscreen? %d\n", c->fullscr);
	printf("Blanking color: R=%2.2f G=%2.2f B=%2.2f\n", c->clr_r, c->clr_g, c->clr_b);
	printf("Title: %s\n", c->title);

	printf("\nImage Paths:\n");
	printf("Frame: %s\n",c->frame_path); 
	printf("Overlay: %s\n",c->ovr_path); 
	printf("Exit Warning: %s\n",c->exit_path);
	printf("Introduction: %s\n",c->intro_path);

	printf("\nShader Paths:\n");
	printf("Tile vertex: %s\n", c->tile_v);
	printf("Tile fragment: %s\n",c->tile_f);
	printf("BG vertex: %s\n",c->bg_v);
	printf("BG fragment: %s\n",c->bg_f);
	printf("Overlay vertex: %s\n",c->hud_v);
	printf("Overlay fragment: %s\n",c->hud_f);

	printf("\nSound Paths:\n");
	printf("BGM: %s\n",c->bgm); 
	printf("Move: %s\n",c->m_snd); 
	printf("Select: %s\n",c->s_snd);
	printf("Rejection: %s\n",c->r_snd);

	printf("\nSound Volumes:\n");
	printf("Overall: %2.2d\n",c->vol); 
	printf("BGM: %2.2d\n",c->bgm_vol); 
	printf("Move: %2.2d\n",c->m_vol); 
	printf("Select: %2.2d\n",c->s_vol);

	printf("\nAnimation Constants:\n");
	printf("Transition frames: %d\n", c->anim_frames);
	printf("Shutdown Time (Millis) %d\n", c->shutdown_time);
	printf("Play intro? %d\n", c->play_intro);

	printf("\nTile Attribute Names:\n");
	printf("Game Node Name: %s\n",c->nm);
	printf("Active Path: %s\n",c->ap);
	printf("Inactive Path: %s\n",c->ip);
	printf("BG Path: %s\n",c->bp);
	printf("Name: %s\n",c->n);
	printf("Command: %s\n",c->c);
	printf("Description: %s\n",c->d);
	printf("ID: %s\n",c->i);
	printf("North ID: %s\n",c->ni);
	printf("South ID: %s\n",c->si);
	printf("East ID: %s\n",c->ei);
	printf("West ID: %s\n",c->wi);
	printf("Has Frame?: %s\n",c->hf);
	printf("Is First?: %s\n",c->is);
	printf("X location: %s\n",c->x);
	printf("Y location: %s\n",c->y);
	printf("Active W: %s\n",c->sxa);
	printf("Active H: %s\n",c->sya);
	printf("Inactive W: %s\n",c->sxi);
	printf("Inactive H: %s\n",c->syi);
	printf("Selectable?: %s\n",c->s);

	printf("\nButton Assignments:\n");
	printf("Shutdown A: %s\n", &c->sdk_a);
	printf("Shutdown B: %s\n", &c->sdk_b);
	printf("Fast Shutdown: %s\n", &c->sdk_f);
	printf("Selection: %s\n", &c->select);
	printf("Windowed Mode: %s\n", &c->window);
	printf("Next Config: %s\n", &c->n_cfg);
	printf("Previous Config: %s\n", &c->p_cfg);
}
