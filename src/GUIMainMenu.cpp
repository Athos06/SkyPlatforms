#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUIMainMenu.h"
#include "GUICredits.h"
#include "AudioManager.h"
#include "Constants.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIMainMenu* Ogre::Singleton<GUIMainMenu>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIMainMenu& GUIMainMenu::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIMainMenu* GUIMainMenu::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIMainMenu::GUIMainMenu(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIMainMenu::~GUIMainMenu(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIMainMenu::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIMainMenu::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetMainMenu;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIMainMenu::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIMainMenu::resume(){
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();

}
//|||||||||||||||||||||||||||||||||||||||||||||||


bool GUIMainMenu::onBackButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAppState();
	return true;
}

bool GUIMainMenu::onPlayButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	
	pushGUIState(findByName("GUISelectLevel"));
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIMainMenu::onExitButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->shutdown();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIMainMenu::onCreditsButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
			m_pAppStateManager->findByName("CreditsState"));
	//pushGUIState(findByName("GUICredits"));
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||


bool GUIMainMenu::onOptionsButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	pushGUIState(findByName("GUIOptions"));
	return true;
}
