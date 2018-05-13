#ifndef GUI_AUDIO_H
#define GUI_AUDIO_H

#include "GUIState.h"

class GUIManager;

class GUIAudio : public Ogre::Singleton<GUIAudio>, GUIState 
{
public:
	
	static GUIAudio& getSingleton();
	static GUIAudio* getSingletonPtr();
	
	GUIAudio();
	~GUIAudio();
	
	DECLARE_GUISTATE_CLASS(GUIAudio)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

	bool onBackButton(const CEGUI::EventArgs &args);
	bool musicVolumeChange(const CEGUI::EventArgs &args);
	bool sfxVolumeChange(const CEGUI::EventArgs &args);

		
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
