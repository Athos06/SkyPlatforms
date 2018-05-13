#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "AudioManager.h"
#include "Constants.h"

#include "GUIManager.h"
#include "GUIPauseMenu.h"
#include "GUICredits.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIPauseMenu* Ogre::Singleton<GUIPauseMenu>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIPauseMenu& GUIPauseMenu::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIPauseMenu* GUIPauseMenu::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIPauseMenu::GUIPauseMenu(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIPauseMenu::~GUIPauseMenu(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIPauseMenu::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
	//CEGUI::System::getSingleton().renderGUI();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIPauseMenu::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetPauseMenu;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIPauseMenu::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIPauseMenu::resume(){
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIPauseMenu::onResumeButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	OgreFramework::getSingletonPtr()->m_pAppStateManager->popAppState();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIPauseMenu::onExitButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	OgreFramework::getSingletonPtr()->m_pAppStateManager->shutdown();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIPauseMenu::onOptionsButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	pushGUIState(findByName("GUIOptions"));
	return true;
}

bool GUIPauseMenu::onMainMenuButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("SplashScreenState"));

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		pushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("MenuState"));
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

