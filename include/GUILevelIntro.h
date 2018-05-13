#ifndef GUILEVELINTRO_H
#define GUILEVELINTRO_H

#include "GUIState.h"

class GUIManager;

class GUILevelIntro : public Ogre::Singleton<GUILevelIntro>, GUIState // 
{
public:
	
	static GUILevelIntro& getSingleton();
	static GUILevelIntro* getSingletonPtr();
	
	GUILevelIntro();
	~GUILevelIntro();
	
	DECLARE_GUISTATE_CLASS(GUILevelIntro)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();


private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
