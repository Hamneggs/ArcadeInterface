#pragma once
#ifndef LOADER_LOADED
#define LOADER_LOADED
#include "TileGrid.h"
#include "const.h"
#include "UIConfig.h"
#include <vector>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

/*
Determines whether or not the given c-string contains a whole number.
*/
bool inline isWholeNumber(char * n)
{
	for(unsigned int i = 0; i < strlen(n); i++)
	{
		if(int(n[i]) < 48 || int(n[i]) < 57)
		{
			return false;
		}
	}
	return true;
}

/*
Loads tiles from an XML file created by the editor.
*/
void inline loadTiles(TileGrid * grid, UIConfig * c)
{
	for(unsigned int i = 0; i < c->paths.size(); i++)
	{
		if(c->ext_tile_rep) printf("\nBeginning Tile Load...");
	
		if(c->ext_tile_rep) printf("\nLoading file...");
		printf("\nCurrent filepath: %s\n", c->paths[i]);

		// Load the file usign Rapid XML's file util.
		rapidxml::file<char> f = rapidxml::file<char>(c->paths.at(i));

		if(c->ext_tile_rep) printf("\nCreating raw xml DOM...");
		// Create an XML document to use.
		rapidxml::xml_document<> doc;

		if(c->ext_tile_rep) printf("\nParsing data into raw DOM...");
		// Parse the file string into the XML document.
		doc.parse<0>(f.data());

		// Create an index to keep track of the current Node.
		unsigned int curNode = 0;

		// Create an index to keep track of the current Attribute.
		unsigned int curAttr = 0;

		// Create a boolean to keep track of whether or not the current Tile is first.
		bool first = false;

		if(c->ext_tile_rep) printf("\nBeginning node iteration...");
		// Parse through the DOM and create Tiles.
		rapidxml::xml_node<char> * node = doc.first_node();
		node = node->first_node(c->nm);
		rapidxml::xml_attribute<char> * attr;

		grid->addLayer();
		printf("Changing layers... current: %d, total %d\n", grid->getCurrentLayer(), grid->getNumLayers());
		grid->setLayer(i);
		printf("Changed layers... current: %d, total %d\n", grid->getCurrentLayer(), grid->getNumLayers());

		while( node != NULL)
		{
			first = false;

			Tile * t = new Tile();


			// Go through all the attributes of this node and load their values
			// into the Tile.
			printf("\nLoading Tile #%d...", curNode);
			if(c->ext_tile_rep) printf("\nNode name: %s...", node->name());
		
			if(c->ext_tile_rep) printf("\nImbuing configuration struct...");
			t->init(c);
		
			if(c->ext_tile_rep) printf("\n\tLoading %s", c->is);
			attr = node->first_attribute(c->is);
			if(c->ext_tile_rep) printf(" [value: %s][atoi: %d]", attr->value(), atoi(attr->value()));
			int firstValue = atoi(attr->value());
					if(firstValue == 1)	first = true;
					else			first = false;
			if(c->ext_tile_rep) printf("\n\t\t...done");

			// Now we initialize the Tile's GPU holdings.
			if(c->ext_tile_rep) printf("\n\tInitializing GPU holdings");
			t->initGPUHoldings(grid->getTileProgram());
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->ap);
			attr = node->first_attribute(c->ap);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initImages(attr->value(), NULL, NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->ip);
			attr = node->first_attribute(c->ip);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initImages(NULL, attr->value(), NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->bp);
			attr = node->first_attribute(c->bp);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initImages(NULL, NULL, attr->value());
			if(c->ext_tile_rep) printf("\n\t\t...done");
		
			if(c->ext_tile_rep) printf("\n\tLoading %s", c->n);
			attr = node->first_attribute(c->n);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initInfo(attr->value(), NULL, NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->d);
			attr = node->first_attribute(c->d);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initInfo(NULL, attr->value(), NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->c);
			attr = node->first_attribute(c->c);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initInfo(NULL, NULL, attr->value());
			if(c->ext_tile_rep) printf("\n\t\t...done. Value stored: [%s]", t->getCommand());

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->i);
			attr = node->first_attribute(c->i);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initID(unsigned int(atoi(attr->value())));
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->hf);
			attr = node->first_attribute(c->hf);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			int frameValue = atoi(attr->value());
					if(frameValue == 1)	t->initFrame(true, grid->getFrameTexture());
					else			t->initFrame(false, grid->getFrameTexture());
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->x);
			attr = node->first_attribute(c->x);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initLocation(float(atof(attr->value())), NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->y);
			attr = node->first_attribute(c->y);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initLocation(NULL, (float)atof(attr->value()));
			if(c->ext_tile_rep) printf("\n\t\t...done");
					
			if(c->ext_tile_rep) printf("\n\tLoading %s", c->sxa);
			attr = node->first_attribute(c->sxa);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes((float)atof(attr->value()), NULL, NULL, NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");
					
			if(c->ext_tile_rep) printf("\n\tLoading %s", c->sya);
			attr = node->first_attribute(c->sya);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes(NULL, (float)atof(attr->value()), NULL, NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->sxi);
			attr = node->first_attribute(c->sxi);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes(NULL, NULL, (float)atof(attr->value()), NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");
					
			if(c->ext_tile_rep) printf("\n\tLoading %s", c->syi);
			attr = node->first_attribute(c->syi);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes(NULL, NULL, NULL, (float)atof(attr->value()));
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tInitializing Animation constants");
			if(c->ext_tile_rep) printf("\n\tLoading %s", c->s);
			attr = node->first_attribute(c->s);
			bool sel = false;
			if(atoi(attr->value()) == 1) sel = true;
			if(c->ext_tile_rep) printf(" [value: %i]", sel);
			t->initAnim(sel);
			if(c->ext_tile_rep) printf("\n\t\t...done");
		
			// Note that we store in the neighbor pointers the value of the ID OF THE NEIGHBOR.
			// Later on we use this value to look for the Tile in the grid and 
			// assign it as the neighbor in-situ.
			if(c->ext_tile_rep) printf("\n\tLoading neighbors...");
			if(c->ext_tile_rep) printf("\n\tLoading %s", c->ni);
			attr = node->first_attribute(c->ni);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors((Tile*)unsigned int(atoi(attr->value())), NULL, NULL, NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->si);
			attr = node->first_attribute(c->si);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors(NULL, (Tile*)unsigned int(atoi(attr->value())), NULL, NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->ei);
			attr = node->first_attribute(c->ei);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors(NULL, NULL, (Tile*)unsigned int(atoi(attr->value())), NULL);
			if(c->ext_tile_rep) printf("\n\t\t...done");

			if(c->ext_tile_rep) printf("\n\tLoading %s", c->wi);
			attr = node->first_attribute(c->wi);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors(NULL, NULL, NULL, (Tile*)unsigned int(atoi(attr->value())));
			if(c->ext_tile_rep) printf("\n\t\t...done");

			// Now that the fucker is initialized (mostly), we 
			// can add it to our intermediate vector.
			grid->addTile(t, first);
			
			curNode++;
			if(c->ext_tile_rep) printf("\nShifting nodes");
			node = node->next_sibling(c->nm);
		}
		// Remember how we stored the ID values of the Tiles' neighbors as the pointers to the neighbors?
		std::vector<Tile*> * tiles = grid->getTiles();
		printf("All [%d] tiles loaded into the current grid layer.\n", tiles->size());
		for(unsigned int i = 0; i < tiles->size(); i ++)
		{
			Tile * tile = tiles->at(i);
			tiles->at(i)->initNeighbors(
				grid->getTile((unsigned int)tile->getNeighbor(NORTH)),
				grid->getTile((unsigned int)tile->getNeighbor(SOUTH)),
				grid->getTile((unsigned int)tile->getNeighbor(EAST)),
				grid->getTile((unsigned int)tile->getNeighbor(WEST))
				);
		}
	}
	for(unsigned int i = 0; i < grid->getNumLayers(); i++)
	{   
		grid->setLayer(i);
		std::vector<Tile *> * tiles = grid->getTiles();
		printf("Grid Layer #%d has %d Tiles. These are those that are contained: \n", grid->getCurrentLayer(), tiles->size());
		for(unsigned int i = 0; i < tiles->size(); i ++)
		{
			printf("%s\n", tiles->at(i)->getName());
		}
	}
	grid->setLayer(0);
	printf("Layer: %d\nP: %10s\nC:%10s\n", grid->getCurrentLayer(), grid->getPrevious()->getName(), grid->getCurrent()->getName());
}

void inline loadUIProperties(UIConfig * c)
{
	c->paths = std::vector<char *>();
	printf("\nLoading properties file...\n");

	// Create a DOM object that we can parse through out of the config file.
	rapidxml::file<char> f = rapidxml::file<char>(CONFIG_FILE_PATH);
	rapidxml::xml_document<> doc;
	doc.parse<0>(f.data());

	// Get the first node of the DOM so we can have a point of reference for navigation.
	rapidxml::xml_node<char> * node = doc.first_node();
	rapidxml::xml_attribute<char> * attr;

	// Create a swapspace int to load booleans from integer values.
	int swapint = 0;

	// Create a swapspace C-String for memory allocation.
	char * swapstr;

//=========================================================================================
// Tile and shader loading preferences.
//=========================================================================================
	
	// Go to the loading properties node so we can load the extended_report preferences.
	printf("==LOADING SHADER AND TILE REPORT PREFERENCES===================================");
	node = node->first_node(LOAD_NODE);
	printf("\nNODE NAME: %s\n", node->name());
	
	// Load the extended shader report.
	printf("Loading extended shader reporting preference...");
	attr = node->first_attribute(EXT_SHD_ATTR);
	swapint = atoi(attr->value());
	if(swapint == 1) c->ext_shader_rep = true;
	else c->ext_shader_rep = false;
	printf("value = %d\n", swapint);

	// Load the extended Tile report.
	printf("Loading extended Tile load reporting preference...");
	attr = node->first_attribute(EXT_TILE_ATTR);
	swapint = atoi(attr->value());
	if(swapint == 1) c->ext_tile_rep = true;
	else c->ext_tile_rep = false;
	printf("value = %d\n", swapint);

//=========================================================================================
// Animation preferences.
//=========================================================================================
	
	// Go to the loading properties node so we can load the extended_report preferences.
	printf("==ANIMATION PREFERENCES========================================================");
	node = node->next_sibling(ANIM_NODE);
	printf("\nNODE NAME: %s\n", node->name());

	// Load the number of frames used to transition between tiles.
	printf("Loading animation time (in frames)...");
	attr = node->first_attribute(ANIM_FRAMES_ATTR);
	c->anim_frames = atoi(attr->value());
	printf("value = %d\n", c->anim_frames);

	// Load the time taken to shut down.
	printf("Loading shutdown time (in milliseconds)...");
	attr = node->first_attribute(SDT_ATTR);
	c->shutdown_time = atoi(attr->value());
	printf("value = %d\n", c->shutdown_time);

	// Load whether or not to play the intro.
	printf("Loading whether or not to play intro...");
	attr = node->first_attribute(PL_INTR_ATTR);
	swapint = atoi(attr->value());
	if(swapint == 1) c->play_intro = true;
	else c->play_intro = false;
	printf("value = %d\n", swapint);


//=========================================================================================
// Sound filepaths and properties.
//=========================================================================================

	// Go to the sound properties node so we can load the extended_report preferences.
	printf("==MUSIC AND SOUND==============================================================");
	node = node->parent();
	node = node->first_node(SND_NODE);
	printf("\nNODE NAME: %s\n", node->name());

	// Load the BGM path.
	printf("Loading BGM file path...");
	attr = node->first_attribute(BGM_ATTR);
	swapstr = attr->value();
	c->bgm = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->bgm, swapstr);
	printf(" ...done\n");
	
	// Load the movement sound path.
	printf("Loading movement sound file path...");
	attr = node->first_attribute(M_SND_ATTR);
	swapstr = attr->value();
	c->m_snd = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->m_snd, swapstr);
	printf(" ...done\n");

	// Load the selection sound path.
	printf("Loading selection sound file path...");
	attr = node->first_attribute(S_SND_ATTR);
	swapstr = attr->value();
	c->s_snd = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->s_snd, swapstr);
	printf(" ...done\n");

	// Load the rejection sound path.
	printf("Loading rejection sound file path...");
	attr = node->first_attribute(R_SND_ATTR);
	swapstr = attr->value();
	c->r_snd = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->r_snd, swapstr);
	printf(" ...done\n");

	// Load the main volume.
	printf("Loading main volume...");
	attr = node->first_attribute(VOL_ATTR);
	c->vol = float(atof(attr->value()));
	printf(" ...done\n");

	// Load the BGM volume.
	printf("Loading BGM volume...");
	attr = node->first_attribute(BGM_VOL_ATTR);
	c->bgm_vol = float(atof(attr->value()));
	printf("value = %f\n", c->bgm_vol);

	// Load the movement sound volume.
	printf("Loading movement sound volume...");
	attr = node->first_attribute(M_VOL_ATTR);
	c->m_vol = float(atof(attr->value()));
	printf("value = %f\n", c->m_vol);

	// Load the selection sound volume.
	printf("Loading selection sound volume...");
	attr = node->first_attribute(S_VOL_ATTR);
	c->s_vol = float(atof(attr->value()));
	printf("value = %f\n", c->s_vol);

	// Load the rejection sound volume.
	printf("Loading rejection sound volume...");
	attr = node->first_attribute(R_VOL_ATTR);
	c->r_vol = float(atof(attr->value()));
	printf("value = %f\n", c->r_vol);


//=========================================================================================
// Window properties.
//=========================================================================================

	// Go to the window properties node so we can load its properties as well.
	printf("==LOADING WINDOW PROPERTIES====================================================");
	node = node->parent();
	node = node->first_node(WIN_NODE);
	printf("\nNODE NAME: %s\n", node->name());

	// Load the X Resolution of the application.
	printf("Loading X resolution...");
	attr = node->first_attribute(X_RES_ATTR);
	c->x_res = atoi(attr->value());
	printf("value = %d\n", c->x_res);
	
	// Load the Y Resolution of the application.
	printf("Loading Y resolution...");
	attr = node->first_attribute(Y_RES_ATTR);
	c->y_res = atoi(attr->value());
	printf("value = %d\n", c->y_res);

	// Load the window location X coordinate.
	printf("Loading window X location...");
	attr = node->first_attribute(WIN_X_ATTR);
	c->win_x = atoi(attr->value());
	printf("value = %d\n", c->win_x);

	// Load the window location Y coordinate.
	printf("Loading window X location...");
	attr = node->first_attribute(WIN_Y_ATTR);
	c->win_y = atoi(attr->value());
	printf("value = %d\n", c->win_y);

	// Load the application's fullscreen status.
	printf("Loading fullscreen status...");
	attr = node->first_attribute(FS_ATTR);
	swapint = atoi(attr->value());
	if(swapint == 0) c->fullscr = false;
	else c->fullscr = true;
	printf("value = %d\n", swapint);

	// Load the application's window blanking color.
	printf("Loading blanking color...");
	attr = node->first_attribute(CLR_R_ATTR);
	c->clr_r = float(atof(attr->value()));
	attr = node->first_attribute(CLR_G_ATTR);
	c->clr_g = float(atof(attr->value()));
	attr = node->first_attribute(CLR_B_ATTR);
	c->clr_b = float(atof(attr->value()));
	printf("R = %1.3f G = %1.3f B = %1.3f\n", c->clr_r, c->clr_g, c->clr_b);

	// Load the window's pointless easter-egg title.
	printf("Loading window title...");
	attr = node->first_attribute(TITLE_ATTR);
	swapstr = attr->value();
	c->title = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->title, swapstr);
	printf(" ...done\n");

//=========================================================================================
// Vital texture loading.
//=========================================================================================

	// Go to the vital textures node so we can load the paths to the application's
	// vital textures.
	printf("==LOADING VITAL TEXTURE PATHS==================================================\n");
	node = node->parent();
	node = node->first_node(TEX_NODE);

	// Load the filepath of the frame texture.
	printf("Loading tile frame texture path...");
	attr = node->first_attribute(FRAME_TEX_ATTR);
	swapstr = attr->value();
	c->frame_path = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->frame_path, swapstr);
	printf(" ...done\n");

	// Load the filepath of the exit warning texture.
	printf("Loading exit warning texture path...");
	attr = node->first_attribute(EXIT_ATTR);
	swapstr = attr->value();
	c->exit_path = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->exit_path, swapstr);
	printf(" ...done\n");

	// Load the filepath of the frame texture.
	printf("Loading overlay texture path...");
	attr = node->first_attribute(OVR_ATTR);
	swapstr = attr->value();
	c->ovr_path = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->ovr_path, swapstr);
	printf(" ...done\n");

	// Load the filepath of the intro overlay texture.
	printf("Loading overlay intro texture path...");
	attr = node->first_attribute(INTR_ATTR);
	swapstr = attr->value();
	c->intro_path = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->intro_path, swapstr);
	printf(" ...done\n");

//=========================================================================================
// Shader file paths.
//=========================================================================================
	
	// Go to the shader paths node so we can load the paths to the application's
	// vital textures.
	printf("==LOADING SHADER SOURCE FILE PATHS=============================================\n");
	node = node->parent();
	node = node->first_node(SHD_NODE);

	// Load the vertex shader of the background.
	printf("Loading BG vertex shader path...");
	attr = node->first_attribute(BGV_ATTR);
	swapstr = attr->value();
	c->bg_v = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->bg_v, swapstr);
	printf(" ...done\n");

	// Load the fragment shader of the background.
	printf("Loading BG fragment shader path...");
	attr = node->first_attribute(BGF_ATTR);
	swapstr = attr->value();
	c->bg_f = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->bg_f, swapstr);
	printf(" ...done\n");

	// Load the vertex shader of the hud.
	printf("Loading HUD vertex shader path...");
	attr = node->first_attribute(HUDV_ATTR);
	swapstr = attr->value();
	c->hud_v = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->hud_v, swapstr);
	printf(" ...done\n");

	// Load the fragment shader of the hud.
	printf("Loading HUD fragment shader path...");
	attr = node->first_attribute(HUDF_ATTR);
	swapstr = attr->value();
	c->hud_f = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->hud_f, swapstr);
	printf(" ...done\n");

	// Load the vertex shader path of the Tile.
	printf("Loading Tile vertex shader path...");
	attr = node->first_attribute(TILEV_ATTR);
	swapstr = attr->value();
	c->tile_v = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->tile_v, swapstr);
	printf(" ...done\n");

	// Load the fragment shader path of the Tile.
	printf("Loading Tile fragment shader path...");
	attr = node->first_attribute(TILEF_ATTR);
	swapstr = attr->value();
	c->tile_f = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->tile_f, swapstr);
	printf(" ...done\n");

//=========================================================================================
// Tile attribute names.
//=========================================================================================
	
	// Now we need to go to the Tile attributes node.
	printf("==LOADING TILE NODE ATTRIBUTE NAMES============================================\n");
	node = node->parent();
	node = node->first_node(TILE_ATTR_NODE);

	// Game node name.
	printf("Game node name...");
	attr = node->first_attribute(NN_ATTR);
	swapstr = attr->value();
	c->nm = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->nm, swapstr);
	printf(" ...done\n");

	// active path.
	printf("Active path attribute name...");
	attr = node->first_attribute(AP_ATTR);
	swapstr = attr->value();
	c->ap = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->ap, swapstr);
	printf(" ...done\n");

	// Inactive path.
	printf("Inactive path attribute name...");
	attr = node->first_attribute(IP_ATTR);
	swapstr = attr->value();
	c->ip = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->ip, swapstr);
	printf(" ...done\n");

	// BG path.
	printf("BG path attribute name...");
	attr = node->first_attribute(BP_ATTR);
	swapstr = attr->value();
	c->bp = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->bp, swapstr);
	printf(" ...done\n");

	// Name.
	printf("Tile name attribute name...");
	attr = node->first_attribute(N_ATTR);
	swapstr = attr->value();
	c->n = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->n, swapstr);
	printf(" ...done\n");

	// Command.
	printf("Tile command attribute name...");
	attr = node->first_attribute(C_ATTR);
	swapstr = attr->value();
	c->c = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->c, swapstr);
	printf(" ...done\n");

	// Description.
	printf("Tile description attribute name...");
	attr = node->first_attribute(D_ATTR);
	swapstr = attr->value();
	c->d = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->d, swapstr);
	printf(" ...done\n");

	// ID.
	printf("Tile ID attribute name...");
	attr = node->first_attribute(I_ATTR);
	swapstr = attr->value();
	c->i = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->i, swapstr);
	printf(" ...done\n");

	// North ID
	printf("Tile north neighbor ID attribute name...");
	attr = node->first_attribute(NI_ATTR);
	swapstr = attr->value();
	c->ni = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->ni, swapstr);
	printf(" ...done\n");

	// South ID.
	printf("Tile south neighbor ID attribute name...");
	attr = node->first_attribute(SI_ATTR);
	swapstr = attr->value();
	c->si = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->si, swapstr);
	printf(" ...done\n");

	// East ID.
	printf("Tile east neighbor ID attribute name...");
	attr = node->first_attribute(EI_ATTR);
	swapstr = attr->value();
	c->ei = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->ei, swapstr);
	printf(" ...done\n");

	// West ID.
	printf("Tile west neighbor ID attribute name...");
	attr = node->first_attribute(WI_ATTR);
	swapstr = attr->value();
	c->wi = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->wi, swapstr);
	printf(" ...done\n");

	// Has frame.
	printf("Tile has frame attribute name...");
	attr = node->first_attribute(HF_ATTR);
	swapstr = attr->value();
	c->hf = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->hf, swapstr);
	printf(" ...done\n");

	// Selectability.
	printf("Tile selectability attribute name...");
	attr = node->first_attribute(SELABLE_ATTR);
	swapstr = attr->value();
	c->s = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->s, swapstr);
	printf(" ...done\n");


	// Is first.
	printf("Loading Tile fragment shader path...");
	attr = node->first_attribute(IS_ATTR);
	swapstr = attr->value();
	c->is = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->is, swapstr);
	printf(" ...done\n");

	// X location.
	printf("Tile X location attribute name...");
	attr = node->first_attribute(X_ATTR);
	swapstr = attr->value();
	c->x = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->x, swapstr);
	printf(" ...done\n");

	// Y location.
	printf("Tile Y location attribute name...");
	attr = node->first_attribute(Y_ATTR);
	swapstr = attr->value();
	c->y = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->y, swapstr);
	printf(" ...done\n");

	// SX active.
	printf("Tile SXA attribute name...");
	attr = node->first_attribute(SXA_ATTR);
	swapstr = attr->value();
	c->sxa = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->sxa, swapstr);
	printf(" ...done\n");

	// SY active.
	printf("Tile SXA attribute name...");
	attr = node->first_attribute(SYA_ATTR);
	swapstr = attr->value();
	c->sya = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->sya, swapstr);
	printf(" ...done\n");

	// SX inactive.
	printf("Tile SXI attribute name...");
	attr = node->first_attribute(SXI_ATTR);
	swapstr = attr->value();
	c->sxi = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->sxi, swapstr);
	printf(" ...done\n");

	// SY inactive.
	printf("Tile SYI attribute name...");
	attr = node->first_attribute(SYI_ATTR);
	swapstr = attr->value();
	c->syi = (char *) malloc( (sizeof(char)*strlen(swapstr))+1);
	strcpy(c->syi, swapstr);
	printf(" ...done\n");
	
//=========================================================================================
// Button assignments.
//=========================================================================================
	
	// Go to the button assignment node in the DOM.
	printf("==LOADING BUTTON ASSIGNMENTS===================================================\n");
	//node = node->parent();
	node = node->next_sibling(BUTTON_NODE);

	// First shutdown key.
	printf("Shutdown key A...");
	attr = node->first_attribute(SDK_A_ATTR);
	/*if(isWholeNumber(attr->value()))
	{
		c->sdk_a = char(atoi(attr->value()));
	}*/
	c->sdk_a = attr->value()[0];
	printf(" ...done\n");
	//printf(" value = \"%s\" [%d]\n", (char)c->sdk_a, (int)c->sdk_a);


	// Second shutdown key.
	printf("Shutdown key B...");
	attr = node->first_attribute(SDK_B_ATTR);
	if(isWholeNumber(attr->value()))
	{
		c->sdk_b = char(atoi(attr->value()));
	}
	c->sdk_b = attr->value()[0];
	printf(" ...done\n");
	//printf(" value = \"%s\" [%d]\n", (char)c->sdk_b, (int)c->sdk_b);

	// Select key.
	printf("Select key...");
	attr = node->first_attribute(SEL_KEY_ATTR);
	if(isWholeNumber(attr->value()))
	{
		c->select = char(atoi(attr->value()));
	}
	c->select = attr->value()[0];
	printf(" ...done\n");
	//printf(" value = \"%s\" [%d]\n", (char)c->select, (int)c->select);

	// Windowing key.
	printf("Windowing key...");
	attr = node->first_attribute(WIN_ATTR);
	if(isWholeNumber(attr->value()))
	{
		c->window = char(atoi(attr->value()));
	}
	c->window = attr->value()[0];
	printf(" ...done\n");
	//printf(" value = \"%s\" [%d]\n", (char)c->window, (int)c->window);

	// Fast shutdown key.
	printf("Fast shutdown key...");
	attr = node->first_attribute(SDK_F_ATTR);
	if(isWholeNumber(attr->value()))
	{
		c->sdk_f = char(atoi(attr->value()));
	}
	c->sdk_f = attr->value()[0];
	printf(" ...done\n");
	//printf(" value = \"%s\" [%d]\n", (char)c->sdk_f, (int)c->sdk_f);

	// Next config key.
	printf("Next config key...");
	attr = node->first_attribute(NEXT_ATTR);
	if(isWholeNumber(attr->value()))
	{
		c->n_cfg = char(atoi(attr->value()));
	}
	c->n_cfg = attr->value()[0];
	printf(" ...done\n");
	//printf(" value = \"%s\" [%d]\n", (char)c->n_cfg, (int)c->n_cfg);

	// Previous config key.
	printf("Previous config key...");
	attr = node->first_attribute(PREV_ATTR);
	if(isWholeNumber(attr->value()))
	{
		c->p_cfg = char(atoi(attr->value()));
	}
	c->p_cfg = attr->value()[0];
	printf(" ...done\n");
	//printf(" value = \"%s\" [%d]\n", (char)c->p_cfg, (int)c->p_cfg);

//=========================================================================================
// Tile arrangement config file paths.
//=========================================================================================
	
	// Swap to the first config file path node.
	printf("==LOADING CONFIGURATION FILE PATHS=============================================\n");
	node = node->parent();
	node = node->first_node(CONFIG_NODE);
	
	printf("Loading config file paths...\n");
	while( node != NULL)
	{
		printf("%8d: ", c->paths.size());
		char* path = (char*) malloc(sizeof(char)*strlen(node->first_attribute(PATH_ATTR)->value()));
		strcpy(path, node->first_attribute(PATH_ATTR)->value());
		c->paths.push_back(path);
		node = node->next_sibling(CONFIG_NODE);
		printf("%s\n", c->paths[c->paths.size()-1]);
	}
	printf("...Loaded %d paths.\n", c->paths.size());

	if(c->ext_tile_rep)
	{
		printf("===============================================================================\n");
		printf("DATA LOADED IS AS FOLLOWS:\n");
		printConfig(c);
	}
}
#endif