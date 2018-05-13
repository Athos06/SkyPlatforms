#ifndef GUI_CONTROLS_H
#define GUI_CONTROLS_H

#include "GUIState.h"

class GUIManager;

class GUIControls : public Ogre::Singleton<GUIControls>, GUIState 
{
public:
	
	static GUIControls& getSingleton();
	static GUIControls* getSingletonPtr();
	
	GUIControls();
	~GUIControls();
	
	DECLARE_GUISTATE_CLASS(GUIControls)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();

	bool onBackButton(const CEGUI::EventArgs &args);
	bool onXInvertCheck(const CEGUI::EventArgs &args);
	bool onYInvertCheck(const CEGUI::EventArgs &args);
	bool onSensitivity(const CEGUI::EventArgs &args);
	
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

#endif
