#ifndef GUI_GAME_H
#define GUI_GAME_H

#include "GUIState.h"

class GUIManager;

class GUIGame : public Ogre::Singleton<GUIGame>, GUIState // 
{
public:
	
	static GUIGame& getSingleton();
	static GUIGame* getSingletonPtr();
	
	GUIGame();
	~GUIGame();
	
	DECLARE_GUISTATE_CLASS(GUIGame)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

	/*bool onPlayButton(const CEGUI::EventArgs &args);
	bool onExitButton(const CEGUI::EventArgs &args);
	bool onCreditsButton(const CEGUI::EventArgs &args);
	*/

private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
