#ifndef GUI_LOADING_H
#define GUI_LOADING_H

#include "GUIState.h"

class GUIManager;

class GUILoading : public Ogre::Singleton<GUILoading>, GUIState // 
{
public:
	
	static GUILoading& getSingleton();
	static GUILoading* getSingletonPtr();
	
	GUILoading();
	~GUILoading();
	
	DECLARE_GUISTATE_CLASS(GUILoading)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
