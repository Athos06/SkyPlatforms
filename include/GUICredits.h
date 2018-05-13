#ifndef GUI_CREDITS_H
#define GUI_CREDITS_H

#include "GUIState.h"

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>


class GUIManager;

class GUICredits : public Ogre::Singleton<GUICredits>, GUIState 
{
public:
	
	static GUICredits& getSingleton();
	static GUICredits* getSingletonPtr();
	
	GUICredits();
	~GUICredits();
	
	DECLARE_GUISTATE_CLASS(GUICredits)

	void enter();
	void exit();
	void loadCEGUI();
	
	bool onBackButton(const CEGUI::EventArgs &args);
	
private:
	CEGUI::Window*	m_pGUIsheet;
	GUIManager* m_pGUImgr;
};

using namespace std;

class CreditsHelper : public Ogre::Singleton<CreditsHelper>{
public:

	static CreditsHelper& getSingleton();
	static CreditsHelper* getSingletonPtr();

	CreditsHelper();
	~CreditsHelper();

	void update(double deltaTime);
	void loadCredits();
	
	void exitCredits();

	bool hasFinished(){return m_bFinished;};

private:
	bool m_bFinished;

	bool m_bExit;

	bool m_bFadeIn;
	bool m_bFadeOut;
	bool m_keepThere;
	bool m_bStop;
	float m_timer;
	float m_alphaValue;

	int m_activeWindow;
	//CEGUI::Window creditos[4][1]; 
		//m_creditsText;
	vector<CEGUI::Window *> m_creditsText;

	CEGUI::Window *m_creditsGUIsheet;
	//string creditsTest;
};


#endif
