#ifndef GUI_GAMECOMPLETE_H
#define GUI_GAMECOMPLETE_H

#include "GUIState.h"

class GUIManager;

class GUIGameComplete : public Ogre::Singleton<GUIGameComplete>, public GUIState{
public:
	static GUIGameComplete& getSingleton();
	static GUIGameComplete* getSingletonPtr();

	GUIGameComplete();
	~GUIGameComplete();
	
	DECLARE_GUISTATE_CLASS(GUIGameComplete)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;

};
#endif