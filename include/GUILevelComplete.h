#ifndef GUI_LEVELCOMPLETE_H
#define GUI_LEVELCOMPLETE_H

#include "GUIState.h"

class GUIManager;

class GUILevelComplete : public Ogre::Singleton<GUILevelComplete>, public GUIState{
public:
	static GUILevelComplete& getSingleton();
	static GUILevelComplete* getSingletonPtr();

	GUILevelComplete();
	~GUILevelComplete();
	
	DECLARE_GUISTATE_CLASS(GUILevelComplete)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();
	
	bool onContinueButton(const CEGUI::EventArgs &args);
	bool onExitButton(const CEGUI::EventArgs &args);
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;

};
#endif