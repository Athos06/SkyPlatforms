#ifndef GUI_MAIN_MENU_H
#define GUI_MAIN_MENU_H

#include "GUIState.h"

class GUIManager;

class GUIMainMenu : public Ogre::Singleton<GUIMainMenu>, GUIState // 
{
public:
	
	static GUIMainMenu& getSingleton();
	static GUIMainMenu* getSingletonPtr();
	
	GUIMainMenu();
	~GUIMainMenu();
	
	DECLARE_GUISTATE_CLASS(GUIMainMenu)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

	bool onBackButton(const CEGUI::EventArgs &args);
	bool onPlayButton(const CEGUI::EventArgs &args);
	bool onExitButton(const CEGUI::EventArgs &args);
	bool onCreditsButton(const CEGUI::EventArgs &args);
	bool onOptionsButton(const CEGUI::EventArgs &args);

private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
