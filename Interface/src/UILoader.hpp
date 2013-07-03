#ifndef LOADER_INCLUDED
#define LOADER_INCLUDED
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
	// Iterate through each letter in the c-string.
	for(unsigned int i = 0; i < strlen(n); i++)
	{
		if(int(n[i]) > 47 || int(n[i]) < 58) // Do the ol integer ascii test.
		{
			return false;
		}
	}
	return true;
}

/*
Loads a String from a node and attribute pair into a destination c-string.
*/
inline char * loadString(char * dst, char * attrName, rapidxml::xml_node<char> * node, rapidxml::xml_attribute<char> * attr)
{
	attr = node->first_attribute(attrName);
	dst = (char *) malloc( (sizeof(char)*(strlen(attr->value())+1)));
	strcpy(dst, attr->value());
	return dst;
}

/*
Loads an integer from a node and attribute pair and returns it.
*/
inline int loadInt(char * attrName, rapidxml::xml_node<char> * node, rapidxml::xml_attribute<char> * attr)
{
	attr = node->first_attribute(attrName);
	return atoi(attr->value());
}

/*
Loads an floating point value from a node and attribute pair and returns it.
*/
inline float loadFloat(char * attrName, rapidxml::xml_node<char> * node, rapidxml::xml_attribute<char> * attr)
{
	attr = node->first_attribute(attrName);
	return (float)atof(attr->value());
}

/*
Loads a boolean value from a node and attribute pair and returns it.
*/
inline bool loadBool(char * attrName, rapidxml::xml_node<char> * node, rapidxml::xml_attribute<char> * attr)
{
	return (loadInt(attrName, node, attr) > 0);
}

/*
Loads a character from a node and attribute pair and returns it.
*/
inline char loadChar(char * attrName, rapidxml::xml_node<char> * node, rapidxml::xml_attribute<char> * attr)
{
	attr = node->first_attribute(attrName);
	if(isWholeNumber(attr->value()))
	{
		return char(atoi(attr->value()));
	}
	else return attr->value()[0];
}


/*
Loads tiles from an XML file created by the editor.
*/
void inline loadTiles(TileGrid * grid, UIConfig * c)
{
	rapidxml::xml_node<char> * node = NULL;
	
	rapidxml::xml_attribute<char> * attr = NULL;
	printf("==TILE LOADING=================================================================\n");
	for(unsigned int i = 0; i < c->paths.size(); i++)
	{	
		if(c->ext_tile_rep)printf("==TILE LAYER %2d ===============================================================\n", i);
		if(c->ext_tile_rep) printf("\nLoading file... ");
		if(c->ext_tile_rep)printf("[Current filepath: %s]", c->paths[i]);

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
		node = doc.first_node();
		node = node->first_node(c->nm);
		attr = NULL;

		// Create a new layer for the current Tile layout.
		grid->addLayer();
		grid->setLayer(i);

		while( node != NULL)
		{
			first = false;

			Tile * t = new Tile();


			// Go through all the attributes of this node and load their values
			// into the Tile.
			if(c->ext_tile_rep)printf("\n==Loading TILE #%4d ==========================================================", curNode);
			t->init(c);
		
			if(c->ext_tile_rep) printf("\n-Loading %s", c->is);
			first = loadBool(c->is, node, attr);
			printf(" [value: %d]",first);
			if(c->ext_tile_rep) printf(" ...done");

			// Now we initialize the Tile's GPU holdings.
			if(c->ext_tile_rep) printf("\n-Initializing GPU holdings");
			t->initGPUHoldings(grid->getTileProgram());
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->ap);
			attr = node->first_attribute(c->ap);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initImages(attr->value(), NULL, NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->ip);
			attr = node->first_attribute(c->ip);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initImages(NULL, attr->value(), NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->bp);
			attr = node->first_attribute(c->bp);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initImages(NULL, NULL, attr->value());
			if(c->ext_tile_rep) printf(" ...done");
		
			if(c->ext_tile_rep) printf("\n-Loading %s", c->n);
			attr = node->first_attribute(c->n);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initInfo(attr->value(), NULL, NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->d);
			attr = node->first_attribute(c->d);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initInfo(NULL, attr->value(), NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->c);
			attr = node->first_attribute(c->c);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initInfo(NULL, NULL, attr->value());
			if(c->ext_tile_rep) printf(" ...done.");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->i);
			attr = node->first_attribute(c->i);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			t->initID(unsigned int(atoi(attr->value())));
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->hf);
			attr = node->first_attribute(c->hf);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
			int frameValue = atoi(attr->value());
					if(frameValue == 1)	t->initFrame(true, grid->getFrameTexture());
					else			t->initFrame(false, grid->getFrameTexture());
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->x);
			attr = node->first_attribute(c->x);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initLocation(float(atof(attr->value())), NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->y);
			attr = node->first_attribute(c->y);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initLocation(NULL, (float)atof(attr->value()));
			if(c->ext_tile_rep) printf(" ...done");
					
			if(c->ext_tile_rep) printf("\n-Loading %s", c->sxa);
			attr = node->first_attribute(c->sxa);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes((float)atof(attr->value()), NULL, NULL, NULL);
			if(c->ext_tile_rep) printf(" ...done");
					
			if(c->ext_tile_rep) printf("\n-Loading %s", c->sya);
			attr = node->first_attribute(c->sya);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes(NULL, (float)atof(attr->value()), NULL, NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->sxi);
			attr = node->first_attribute(c->sxi);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes(NULL, NULL, (float)atof(attr->value()), NULL);
			if(c->ext_tile_rep) printf(" ...done");
					
			if(c->ext_tile_rep) printf("\n-Loading %s", c->syi);
			attr = node->first_attribute(c->syi);
			if(c->ext_tile_rep) printf(" [value: %f]", atof(attr->value()));
					t->initSizes(NULL, NULL, NULL, (float)atof(attr->value()));
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->s);
			bool sel = loadBool(c->s, node, attr);
			if(c->ext_tile_rep) printf(" [value: %i]", sel);
			t->initAnim(sel);
			if(c->ext_tile_rep) printf(" ...done");
		
			// Note that we store in the neighbor pointers the value of the ID OF THE NEIGHBOR.
			// Later on we use this value to look for the Tile in the grid and 
			// assign it as the neighbor in-situ.
			if(c->ext_tile_rep) printf("\n-Loading %s", c->ni);
			attr = node->first_attribute(c->ni);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors((Tile*)unsigned int(atoi(attr->value())), NULL, NULL, NULL);
			if(c->ext_tile_rep) printf(" ...done");
			 
			if(c->ext_tile_rep) printf("\n-Loading %s", c->si);
			attr = node->first_attribute(c->si);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors(NULL, (Tile*)unsigned int(atoi(attr->value())), NULL, NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->ei);
			attr = node->first_attribute(c->ei);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors(NULL, NULL, (Tile*)unsigned int(atoi(attr->value())), NULL);
			if(c->ext_tile_rep) printf(" ...done");

			if(c->ext_tile_rep) printf("\n-Loading %s", c->wi);
			attr = node->first_attribute(c->wi);
			if(c->ext_tile_rep) printf(" [value: %s]", attr->value());
					t->initNeighbors(NULL, NULL, NULL, (Tile*)unsigned int(atoi(attr->value())));
			if(c->ext_tile_rep) printf("...done\n");

			// Now that the fucker is initialized (mostly), we 
			// can add it to our intermediate vector.
			grid->addTile(t, first);
			
			curNode++;
			node = node->next_sibling(c->nm);
		}
		// Remember how we stored the ID values of the Tiles' neighbors as the pointers to the neighbors?
		std::vector<Tile*> * tiles = grid->getTiles();
		printf("\nAll [%d] tiles loaded into the current grid layer.\n", tiles->size());
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
		printf("\nGrid Layer #%d has %d Tiles. These are those that are contained: \n", grid->getCurrentLayer(), tiles->size());
		for(unsigned int i = 0; i < tiles->size(); i ++)
		{
			printf("%d: %s\n", tiles->at(i)->getID(), tiles->at(i)->getName());
		}
	}
	grid->setLayer(0);
	printf("===============================================================================\n");
	printf("Done loading tiles.\n\n");

}

void inline loadUIProperties(UIConfig * c)
{
	c->paths = std::vector<char *>();
	printf("\n-Loading properties file...\n");

	// Create a DOM object that we can parse through out of the config file.
	rapidxml::file<char> f = rapidxml::file<char>(CONFIG_FILE_PATH);
	rapidxml::xml_document<> doc;
	doc.parse<0>(f.data());

	// Get the first node of the DOM so we can have a point of reference for navigation.
	rapidxml::xml_node<char> * node = doc.first_node();
	rapidxml::xml_attribute<char> * attr = NULL;

//=========================================================================================
// Tile and shader -Loading preferences.
//=========================================================================================
	
	// Go to the -Loading properties node so we can load the extended_report preferences.
	printf("==SHADER LOADING AND TILE REPORT PREFERENCES===================================\n");
	node = node->first_node(LOAD_NODE);
	printf("NODE NAME: %s\n", node->name());
	
	// Load the extended shader report.
	printf("-Loading extended shader reporting preference...");
	c->ext_shader_rep = loadBool(EXT_SHD_ATTR, node, attr);
	printf("value = %d\n", (int)c->ext_shader_rep);

	// Load the extended Tile report.
	printf("-Loading extended Tile load reporting preference...");
	c->ext_tile_rep = loadBool(EXT_TILE_ATTR, node, attr);
	printf("value = %d\n", (int)c->ext_tile_rep);

//=========================================================================================
// Animation preferences.
//=========================================================================================
	
	// Go to the -Loading properties node so we can load the extended_report preferences.
	printf("==ANIMATION PREFERENCES========================================================\n");
	node = node->next_sibling(ANIM_NODE);
	printf("NODE NAME: %s\n", node->name());

	// Load the number of frames used to transition between tiles.
	printf("-Loading animation time (in frames)...");
	c->anim_frames = loadInt(ANIM_FRAMES_ATTR, node, attr);
	printf("value = %d\n", c->anim_frames);

	// Load the time taken to shut down.
	printf("-Loading shutdown time (in milliseconds)...");
	c->shutdown_time = loadInt(SDT_ATTR, node, attr);
	printf("value = %d\n", c->shutdown_time);

	// Load whether or not to play the intro.
	printf("-Loading whether or not to play intro...");
	c->play_intro = loadBool(PL_INTR_ATTR, node, attr);
	printf("value = %d\n", (int)c->play_intro);


//=========================================================================================
// Sound filepaths and properties.
//=========================================================================================

	// Go to the sound properties node so we can load the extended_report preferences.
	printf("==MUSIC AND SOUND==============================================================\n");
	node = node->parent();
	node = node->first_node(SND_NODE);
	printf("NODE NAME: %s\n", node->name());

	// Load the BGM path.
	printf("-Loading BGM file path...");
	c->bgm = loadString(c->bgm, BGM_ATTR, node, attr);
	printf(" ...done\n");
	
	// Load the movement sound path.
	printf("-Loading movement sound file path...");
	c->m_snd = loadString(c->m_snd, M_SND_ATTR, node, attr);
	printf(" ...done\n");

	// Load the selection sound path.
	printf("-Loading selection sound file path...");
	c->s_snd = loadString(c->s_snd, S_SND_ATTR, node, attr);
	printf(" ...done\n");

	// Load the rejection sound path.
	printf("-Loading rejection sound file path...");
	c->r_snd = loadString(c->r_snd, R_SND_ATTR, node, attr);
	printf(" ...done\n");

	// Load the main volume.
	printf("-Loading main volume...");
	c->vol = loadFloat(VOL_ATTR, node, attr);
	printf(" ...done\n");

	// Load the BGM volume.
	printf("-Loading BGM volume...");
	c->bgm_vol = loadFloat(BGM_VOL_ATTR, node, attr);
	printf("value = %f\n", c->bgm_vol);

	// Load the movement sound volume.
	printf("-Loading movement sound volume...");
	c->m_vol = loadFloat(M_VOL_ATTR, node, attr);
	printf("value = %f\n", c->m_vol);

	// Load the selection sound volume.
	printf("-Loading selection sound volume...");
	c->s_vol = loadFloat(S_VOL_ATTR, node, attr);
	printf("value = %f\n", c->s_vol);

	// Load the rejection sound volume.
	printf("-Loading rejection sound volume...");
	c->r_vol = loadFloat(R_VOL_ATTR, node, attr);
	printf("value = %f\n", c->r_vol);


//=========================================================================================
// Window properties.
//=========================================================================================

	// Go to the window properties node so we can load its properties as well.
	printf("==WINDOW PROPERTIES============================================================\n");
	node = node->parent();
	node = node->first_node(WIN_NODE);
	printf("NODE NAME: %s\n", node->name());

	// Load the X Resolution of the application.
	printf("-Loading X resolution...");
	c->x_res = loadInt(X_RES_ATTR, node, attr);
	printf("value = %d\n", c->x_res);
	
	// Load the Y Resolution of the application.
	printf("-Loading Y resolution...");
	c->y_res = loadInt(Y_RES_ATTR, node, attr);
	printf("value = %d\n", c->y_res);

	// Load the window location X coordinate.
	printf("-Loading window X location...");
	c->win_x = loadInt(WIN_X_ATTR, node, attr);
	printf("value = %d\n", c->win_x);

	// Load the window location Y coordinate.
	printf("-Loading window X location...");
	c->win_y = loadInt(WIN_Y_ATTR, node, attr);
	printf("value = %d\n", c->win_y);

	// Load the application's fullscreen status.
	printf("-Loading fullscreen status...");
	c->fullscr = loadBool(FS_ATTR, node, attr);
	printf("value = %d\n", (int)c->fullscr);

	// Load the application's window blanking color.
	printf("-Loading blanking color...");
	c->clr_r = loadFloat(CLR_R_ATTR, node, attr);
	c->clr_g = loadFloat(CLR_G_ATTR, node, attr);
	c->clr_b = loadFloat(CLR_B_ATTR, node, attr);
	printf("R = %1.3f G = %1.3f B = %1.3f\n", c->clr_r, c->clr_g, c->clr_b);

	// Load the window's pointless easter-egg title.
	printf("-Loading window title...");
	c->title = loadString(c->title, TITLE_ATTR, node, attr);
	printf(" ...done\n");

//=========================================================================================
// Vital texture -Loading.
//=========================================================================================

	// Go to the vital textures node so we can load the paths to the application's
	// vital textures.
	printf("==VITAL TEXTURE PATHS==========================================================\n");
	node = node->parent();
	node = node->first_node(TEX_NODE);
	printf("NODE NAME: %s\n", node->name());

	// Load the filepath of the frame texture.
	printf("-Loading tile frame texture path...");
	c->frame_path = loadString(c->frame_path, FRAME_TEX_ATTR, node, attr);
	printf(" ...done\n");

	// Load the filepath of the exit warning texture.
	printf("-Loading exit warning texture path...");
	c->exit_path = loadString(c->exit_path, EXIT_ATTR, node, attr);
	printf(" ...done\n");

	// Load the filepath of the overlay texture.
	printf("-Loading overlay texture path...");\
	c->ovr_path = loadString(c->ovr_path, OVR_ATTR, node, attr);
	printf(" ...done\n");

	// Load the filepath of the intro overlay texture.
	printf("-Loading overlay intro texture path...");
	c->intro_path = loadString(c->intro_path, INTR_ATTR, node, attr);
	printf(" ...done\n");

//=========================================================================================
// Shader file paths.
//=========================================================================================
	
	// Go to the shader paths node so we can load the paths to the application's
	// vital textures.
	printf("==SHADER SOURCE FILE PATHS=====================================================\n");
	node = node->parent();
	node = node->first_node(SHD_NODE);
	printf("NODE NAME: %s\n", node->name());

	// Load the vertex shader of the background.
	printf("-Loading BG vertex shader path...");
	c->bg_v = loadString(c->bg_v, BGV_ATTR, node, attr);
	printf(" ...done\n");

	// Load the fragment shader of the background.
	printf("-Loading BG fragment shader path...");
	c->bg_f = loadString(c->bg_f, BGF_ATTR, node, attr);
	printf(" ...done\n");

	// Load the vertex shader of the hud.
	printf("-Loading HUD vertex shader path...");
	c->hud_v = loadString(c->hud_v, HUDV_ATTR, node, attr);
	printf(" ...done\n");

	// Load the fragment shader of the hud.
	printf("-Loading HUD fragment shader path...");
	c->hud_f = loadString(c->hud_f, HUDF_ATTR, node, attr);
	printf(" ...done\n");

	// Load the vertex shader path of the Tile.
	printf("-Loading Tile vertex shader path...");
	c->tile_v = loadString(c->tile_v, TILEV_ATTR, node, attr);
	printf(" ...done\n");

	// Load the fragment shader path of the Tile.
	printf("-Loading Tile fragment shader path...");
	c->tile_f = loadString(c->tile_f, TILEF_ATTR, node, attr);
	printf(" ...done\n");

//=========================================================================================
// Tile attribute names.
//=========================================================================================
	
	// Now we need to go to the Tile attributes node.
	printf("==TILE NODE ATTRIBUTE NAMES====================================================\n");
	node = node->parent();
	node = node->first_node(TILE_ATTR_NODE);
	printf("NODE NAME: %s\n", node->name());

	// Game node name.
	printf("-Loading Game node name...");
	c->nm = loadString(c->nm, NN_ATTR, node, attr);
	printf(" ...done\n");

	// active path.
	printf("-Loading Active path attribute name...");
	c->ap = loadString(c->ap, AP_ATTR, node, attr);
	printf(" ...done\n");

	// Inactive path.
	printf("-Loading Inactive path attribute name...");
	c->ip = loadString(c->ip, IP_ATTR, node, attr);
	printf(" ...done\n");

	// BG path.
	printf("-Loading BG path attribute name...");
	c->bp = loadString(c->bp, BP_ATTR, node, attr);
	printf(" ...done\n");

	// Name.
	printf("-Loading Tile name attribute name...");
	c->n = loadString(c->n, N_ATTR, node, attr);
	printf(" ...done\n");

	// Command.
	printf("-Loading Tile command attribute name...");
	c->c = loadString(c->c, C_ATTR, node, attr);
	printf(" ...done\n");

	// Description.
	printf("-Loading Tile description attribute name...");
	c->d = loadString(c->d, D_ATTR, node, attr);
	printf(" ...done\n");

	// ID.
	printf("-Loading Tile ID attribute name...");
	c->i = loadString(c->i, I_ATTR, node, attr);
	printf(" ...done\n");

	// North ID
	printf("-Loading Tile north neighbor ID attribute name...");
	c->ni = loadString(c->ni, NI_ATTR, node, attr);
	printf(" ...done\n");

	// South ID.
	printf("-Loading Tile south neighbor ID attribute name...");
	c->si = loadString(c->si, SI_ATTR, node, attr);
	printf(" ...done\n");

	// East ID.
	printf("-Loading Tile east neighbor ID attribute name...");
	c->ei = loadString(c->ei, EI_ATTR, node, attr);
	printf(" ...done\n");

	// West ID.
	printf("-Loading Tile west neighbor ID attribute name...");
	c->wi = loadString(c->wi, WI_ATTR, node, attr);
	printf(" ...done\n");

	// Has frame.
	printf("-Loading Tile has frame attribute name...");
	c->hf = loadString(c->hf, HF_ATTR, node, attr);
	printf(" ...done\n");

	// Selectability.
	printf("-Loading Tile selectability attribute name...");
	c->s = loadString(c->s, SELABLE_ATTR, node, attr);
	printf(" ...done\n");


	// Is first.
	printf("-Loading Tile fragment shader path...");
	c->is = loadString(c->is, IS_ATTR, node, attr);
	printf(" ...done\n");

	// X location.
	printf("-Loading Tile X location attribute name...");
	c->x = loadString(c->x, X_ATTR, node, attr);
	printf(" ...done\n");

	// Y location.
	printf("-Loading Tile Y location attribute name...");
	c->y = loadString(c->y, Y_ATTR, node, attr);
	printf(" ...done\n");

	// SX active.
	printf("-Loading Tile SXA attribute name...");
	c->sxa = loadString(c->sxa, SXA_ATTR, node, attr);
	printf(" ...done\n");

	// SY active.
	printf("-Loading Tile SXA attribute name...");
	c->sya = loadString(c->sya, SYA_ATTR, node, attr);
	printf(" ...done\n");

	// SX inactive.
	printf("-Loading Tile SXI attribute name...");
	c->sxi = loadString(c->sxi, SXI_ATTR, node, attr);
	printf(" ...done\n");

	// SY inactive.
	printf("-Loading Tile SYI attribute name...");
	c->syi = loadString(c->syi, SYI_ATTR, node, attr);
	printf(" ...done\n");
	
//=========================================================================================
// Button assignments.
//=========================================================================================
	
	// Go to the button assignment node in the DOM.
	printf("==BUTTON ASSIGNMENTS===========================================================\n");
	//node = node->parent();
	node = node->next_sibling(BUTTON_NODE);
	printf("NODE NAME: %s\n", node->name());

	// First shutdown key.
	printf("-Loading Shutdown key A...");
	c->sdk_a = loadChar(SDK_A_ATTR, node, attr);
	printf(" ...done\n");

	// Second shutdown key.
	printf("-Loading Shutdown key B...");
	c->sdk_b = loadChar(SDK_B_ATTR, node, attr);
	printf(" ...done\n");

	// Select key.
	printf("-Loading Select key...");
	c->select = loadChar(SEL_KEY_ATTR, node, attr);
	printf(" ...done\n");

	// Windowing key.
	printf("-Loading Windowing key...");
	c->window = loadChar(WIN_ATTR, node, attr);
	printf(" ...done\n");

	// Fast shutdown key.
	printf("-Loading Fast shutdown key...");
	c->sdk_f = loadChar(SDK_F_ATTR, node, attr);
	printf(" ...done\n");

	// Next config key.
	printf("-Loading Next config key...");
	c->n_cfg = loadChar(NEXT_ATTR, node, attr);
	printf(" ...done\n");

	// Previous config key.
	printf("-Loading Previous config key...");
	c->p_cfg = loadChar(PREV_ATTR, node, attr);
	printf(" ...done\n");

//=========================================================================================
// Tile arrangement config file paths.
//=========================================================================================
	
	// Swap to the first config file path node.
	printf("==CONFIGURATION FILE PATHS=====================================================\n");
	node = node->parent();
	node = node->first_node(CONFIG_NODE);
	
	printf("-Loading config file paths...\n");
	while( node != NULL)
	{
		printf("%8d: ", c->paths.size());
		char * path = NULL;
		path = loadString(path, PATH_ATTR, node, attr);	
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
		printf("===============================================================================\n");
	}
	else
	{
		printf("===============================================================================\n");
		printf("Done -Loading config data.\n\n\n");
	}
}
#endif