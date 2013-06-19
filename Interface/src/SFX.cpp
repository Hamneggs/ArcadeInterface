#include "SFX.h"

namespace audio
{
	// The UIConfig struct that we use to load the sounds and set the volumes.
	UIConfig * c;

	// The IrrKlang sound engine instance that we will be using.
	irrklang::ISoundEngine* engine;

	// The source of the BGM.
	irrklang::ISoundSource * bgmSource;

	// A reference to the sound that is played when we play the BGM so
	// we can stop it if needed.
	irrklang::ISound * bgm;

	// The source of the selection sound.
	irrklang::ISoundSource * selectSource;

	// The source of the movement sound.
	irrklang::ISoundSource * moveSource;

	/*
	Initializes the audio code, loading in the various sound files from files 
	specified in the UIConfig struct.
	*/
	bool init(UIConfig * newConfig)
	{
		// Internalize the config reference.
		c = newConfig;

		// Create the sound engine.
		engine = irrklang::createIrrKlangDevice();

		// If something went wrong creating the engine we return a bad mark.
		if(!engine) return false;

		// Set the overall volume to that specified.
		engine->setSoundVolume(c->vol);

		// Create the BGM sound source, but wait to load it until we actually need to play it.
		bgmSource = engine->addSoundSourceFromFile(c->bgm, irrklang::ESM_AUTO_DETECT, false);
		// Load the selection sound source from a file, preloading it to cut latency. 
		selectSource = engine->addSoundSourceFromFile(c->s_snd, irrklang::ESM_AUTO_DETECT, true);
		// Load the bgm sound source from a file, preloaded as well.
		moveSource = engine->addSoundSourceFromFile(c->m_snd, irrklang::ESM_AUTO_DETECT, true);

		// Set the volumes of the sound elements.
		bgmSource->setDefaultVolume(c->bgm_vol);
		selectSource->setDefaultVolume(c->s_vol);
		moveSource->setDefaultVolume(c->m_vol);
		
		return true;
	}

	/*
	Sets off the BGM!
	*/
	void startBGM(void)
	{
		if((bgm == NULL || bgm->getIsPaused()) && engine != NULL) bgm = engine->play2D(bgmSource, true);
	}

	/*
	Corrals and slaughters the BGM...
	*/
	void stopBGM(void)
	{
		if(!bgm->getIsPaused()) bgm->stop();
		bgm = NULL;
	}

	/*
	Plays the menu movement sound.
	*/
	void playMove(void)
	{
		if(engine != NULL) engine->play2D(moveSource);
	}

	/*
	Plays the tile selection sound.
	*/
	void playSelect(void)
	{
		if(engine != NULL) engine->play2D(selectSource);
	} 
}