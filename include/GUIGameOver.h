#ifndef GUI_GAMEOVER_H
#define GUI_GAMEOVER_H

#include "GUIState.h"

class GUIManager;

class GUIGameOver : public Ogre::Singleton<GUIGameOver>, public GUIState{
public:
	static GUIGameOver& getSingleton();
	static GUIGameOver* getSingletonPtr();

	GUIGameOver();
	~GUIGameOver();
	
	DECLARE_GUISTATE_CLASS(GUIGameOver)

	void enter();
	void exit();
	void loadCEGUI();
	void resume();
	
	void showEndWindow();

	bool onRestartButton(const CEGUI::EventArgs &args);
	bool onContinueButton(const CEGUI::EventArgs &args);
	bool onBackButton(const CEGUI::EventArgs &args);

private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;

};
#endif