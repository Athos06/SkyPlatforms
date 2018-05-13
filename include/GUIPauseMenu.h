#ifndef GUI_PAUSE_MENU_H
#define GUI_PAUSE_MENU_H

#include "GUIState.h"

class GUIManager;

class GUIPauseMenu : public Ogre::Singleton<GUIPauseMenu>, GUIState // 
{
public:
	
	static GUIPauseMenu& getSingleton();
	static GUIPauseMenu* getSingletonPtr();
	
	GUIPauseMenu();
	~GUIPauseMenu();
	
	DECLARE_GUISTATE_CLASS(GUIPauseMenu)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

	bool onResumeButton(const CEGUI::EventArgs &args);
	bool onExitButton(const CEGUI::EventArgs &args);
	bool onOptionsButton(const CEGUI::EventArgs &args);
	bool onMainMenuButton(const CEGUI::EventArgs &args);

private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
