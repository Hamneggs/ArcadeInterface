#include "SFX.h"

namespace audio
{
	// The UIConfig struct that we use to load the sounds and set the volumes.
	UIConfig * c;

	// The IrrKlang sound engine instance that we will be using.
	irrklang::ISoundEngine* engine;

	// The source of the BGM.
	irrklang::ISoundSource * bgmSource;

	// The source of the selection sound.
	irrklang::ISoundSource * selectSource;

	// The source of the movement sound.
	irrklang::ISoundSource * moveSource;

	// The source of the rejection sound.
	irrklang::ISoundSource * rejectionSource;

	// A reference to the sound that is played when we play the BGM so
	// we can stop it if needed.
	irrklang::ISound * bgm;

	// We keep references to the sounds of the SFX to prevent spamming.
	irrklang::ISound * select;
	irrklang::ISound * move;
	irrklang::ISound * rejection;

	// We store a flag that alternates each time the movement sound is played
	// so that we can alternate the speaker it comes out of.
	float xPos;

	/*
	Initializes the audio code, loading in the various sound files from files 
	specified in the UIConfig struct.
	*/
	bool init(UIConfig * newConfig)
	{

		xPos = 2.0f;
		// Internalize the config reference.
		c = newConfig;

		// Create the sound engine.
		engine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT);

		// If something went wrong creating the engine we return a bad mark.
		if(!engine) return false;

		// Set the overall volume to that specified.
		engine->setSoundVolume(c->vol);

		engine->setListenerPosition(irrklang::vec3df(0,0,0), irrklang::vec3df(0,0,1));

		// Create the BGM sound source, but wait to load it until we actually need to play it.
		bgmSource = engine->addSoundSourceFromFile(c->bgm, irrklang::ESM_AUTO_DETECT, false);
		// Load the selection sound source from a file, preloading it to cut latency. 
		selectSource = engine->addSoundSourceFromFile(c->s_snd, irrklang::ESM_AUTO_DETECT, true);
		// Load the movement sound source from a file, preloaded as well.
		moveSource = engine->addSoundSourceFromFile(c->m_snd, irrklang::ESM_AUTO_DETECT, true);
		// Finally we load the source of the rejection sound, preloading it as well.
		rejectionSource = engine->addSoundSourceFromFile(c->r_snd, irrklang::ESM_AUTO_DETECT, true);

		// Set the minimum sound radius of the movement sound.
		moveSource->setDefaultMinDistance(10.0);

		// Set the volumes of the sound elements.
		bgmSource->setDefaultVolume(c->bgm_vol);
		selectSource->setDefaultVolume(c->s_vol);
		moveSource->setDefaultVolume(c->m_vol);
		rejectionSource->setDefaultVolume(c->r_vol);
		
		return true;
	}

	/*
	Sets off the BGM!
	*/
	void startBGM(void)
	{
		if(engine != NULL)
		{
			if(!bgm) bgm = engine->play2D(bgmSource, true, false, true);
			else bgm->setVolume(c->bgm_vol);
		}
	}

	/*
	Corrals and slaughters the BGM...
	*/
	void stopBGM(void)
	{
		if(bgm)bgm->setVolume(0.0);
	}

	/*
	Plays the menu movement sound.
	*/
	void playMove(void)
	{
		if(engine != NULL)
		{ 
			if(!move)
			{
				move = engine->play3D(moveSource, irrklang::vec3df(xPos, 0, 0), false, false, true);
			}
			if(move)
			{
				move->stop();
				move = engine->play3D(moveSource, irrklang::vec3df(xPos, 0, 0), false, false, true);
			}
			xPos*=-1;
			printf("XPOS: %f\n", xPos);
		}
	}

	/*
	Plays the tile selection sound.
	*/
	void playSelect(void)
	{
		if(engine != NULL) engine->play2D(selectSource);
	}

	/*
	Plays the rejected action sound.
	*/
	void playRejection(void)
	{
		if(engine != NULL)
		{ 
			if(!rejection)
			{
				rejection = engine->play2D(rejectionSource, false, false, true);
			}
			if(rejection)
			{
				rejection->stop();
				rejection = engine->play2D(rejectionSource, false, false, true);
			}
		}
	}
}