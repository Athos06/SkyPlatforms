#ifndef GUI_OPTIONS_H
#define GUI_OPTIONS_H

#include "GUIState.h"

class GUIManager;

class GUIOptions : public Ogre::Singleton<GUIOptions>, GUIState 
{
public:
	
	static GUIOptions& getSingleton();
	static GUIOptions* getSingletonPtr();
	
	GUIOptions();
	~GUIOptions();
	
	DECLARE_GUISTATE_CLASS(GUIOptions)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();


	bool onAudioButton(const CEGUI::EventArgs &args);
	bool onControlsButton(const CEGUI::EventArgs &args);
	bool onBackButton(const CEGUI::EventArgs &args);
	

	
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
