#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "GameManager.h"
#include "GUIManager.h"
#include "GUILevelComplete.h"
#include "AudioManager.h"
#include "Constants.h"

template<> GUILevelComplete* Ogre::Singleton<GUILevelComplete>::msSingleton = 0;

GUILevelComplete& GUILevelComplete::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILevelComplete* GUILevelComplete::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}

GUILevelComplete::GUILevelComplete(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILevelComplete::~GUILevelComplete(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILevelComplete::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILevelComplete::loadCEGUI()
{
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow
		("LevelCompleteRoot/ExitButton");
	
	if(GameManager::getSingletonPtr()->isGameComplete()){
		exitButton->setEnabled(false);
	}
	else{
		exitButton->setEnabled(true);
	}
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetLevelComplete;	 
}

void GUILevelComplete::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUILevelComplete::resume(){
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();

}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUILevelComplete::onContinueButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	//si hemos superado el ultimo nivel al pulsar continua no empezamos un nuevo
	//nivel sino que vamos al estado especial GameComplete, donde ejecuto
	//la escena del fin del juego
	if(GameManager::getSingletonPtr()->isGameComplete()){
		OgreFramework::getSingletonPtr()->m_pAppStateManager->
			popAllAndPushAppState(OgreFramework::getSingletonPtr()->
			m_pAppStateManager->findByName("GameCompleteState"));
	}
	else{
		OgreFramework::getSingletonPtr()->m_pAppStateManager->
			popAllAndPushAppState(OgreFramework::getSingletonPtr()->
			m_pAppStateManager->findByName("GameState"));
	}
	return true;
}
bool GUILevelComplete::onExitButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("SplashScreenState"));

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		pushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("MenuState"));
	return true;
}
