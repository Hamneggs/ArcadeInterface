#pragma once
#include "irrKlang.h"
#include "UIConfig.h"

namespace audio
{

	/*
	Initializes the audio code, loading in the various sound files from files 
	specified in the UIConfig struct.
	*/
	bool init(UIConfig * c);

	/*
	Sets off the BGM!
	*/
	void startBGM(void);

	/*
	Corrals and slaughters the BGM...
	*/
	void stopBGM(void);

	/*
	Plays the menu movement sound.
	*/
	void playMove(void);

	/*
	Plays the tile selection sound.
	*/
	void playSelect(void); 

	/*
	Plays the rejected action sound.
	*/
	void playRejection(void);
}
