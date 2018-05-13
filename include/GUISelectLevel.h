#ifndef GUI_SELECTLEVEL_H
#define GUI_SELECTLEVEL_H

#include "GUIState.h"

class GUIManager;

class GUISelectLevel : public Ogre::Singleton<GUISelectLevel>, GUIState 
{
public:
	
	static GUISelectLevel& getSingleton();
	static GUISelectLevel* getSingletonPtr();
	
	GUISelectLevel();
	~GUISelectLevel();
	
	DECLARE_GUISTATE_CLASS(GUISelectLevel)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();


	bool onLvl1Button(const CEGUI::EventArgs &args);
	bool onLvl2Button(const CEGUI::EventArgs &args);
	bool onLvl3Button(const CEGUI::EventArgs &args);
	bool onLvlBossButton(const CEGUI::EventArgs &args);
	bool onBackButton(const CEGUI::EventArgs &args);

	
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
