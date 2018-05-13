#ifndef GUI_SPLASHSCREEN_H
#define GUI_SPLASHSCREEN_H

#include "GUIState.h"

class GUIManager;

class GUISplashScreen : public Ogre::Singleton<GUISplashScreen>, GUIState  
{
public:
	
	static GUISplashScreen& getSingleton();
	static GUISplashScreen* getSingletonPtr();
	
	GUISplashScreen();
	~GUISplashScreen();
	
	DECLARE_GUISTATE_CLASS(GUISplashScreen)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
