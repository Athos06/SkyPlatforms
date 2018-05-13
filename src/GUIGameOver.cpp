#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUIGameOver.h"
#include "GUICredits.h"
#include "AudioManager.h"
#include "Constants.h"
#include "GameManager.h"
 

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIGameOver* Ogre::Singleton<GUIGameOver>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameOver& GUIGameOver::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameOver* GUIGameOver::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}

GUIGameOver::GUIGameOver(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameOver::~GUIGameOver(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGameOver::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();

	showEndWindow();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGameOver::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetGameOver;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||
void GUIGameOver::showEndWindow(){
	CEGUI::Window* ouchImg = CEGUI::WindowManager::getSingleton().
				getWindow("GameOverRoot/Ouch");
	CEGUI::Window * livesLeftText = CEGUI::WindowManager::getSingleton().
				getWindow("GameOverRoot/LivesLeftText");
	CEGUI::Window*  gameOverImg = CEGUI::WindowManager::getSingleton().
		getWindow("GameOverRoot/gameOverImg");
	CEGUI::Window* continueButton =  CEGUI::WindowManager::getSingleton().
				getWindow("GameOverRoot/ContinueButton");
	
	ouchImg->hide();
	livesLeftText->hide();
	gameOverImg->hide();
	continueButton->setEnabled(true);

	int livesLeft = GameManager::getSingletonPtr()->getLives();

	if(livesLeft > 0){
		livesLeftText->setText(Ogre::StringConverter::toString(livesLeft)+
		" "+"LIVES LEFT" );				
		ouchImg->show();
		livesLeftText->show();
	}
	else{
		gameOverImg->show();
		continueButton->setEnabled(false);
	}
}


void GUIGameOver::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIGameOver::resume(){
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();

}
//|||||||||||||||||||||||||||||||||||||||||||||||
bool GUIGameOver::onRestartButton(const CEGUI::EventArgs &args){
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("GameState"));
	return true;
}

bool GUIGameOver::onContinueButton(const CEGUI::EventArgs &args){
	
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	GameManager::getSingletonPtr()->retryLevel();
	//cuando entramos aqui estamos en gameover state, lo que hacemos es volver
	//al game state
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAppState();

	//popGUIState();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIGameOver::onBackButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("SplashScreenState"));

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		pushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("MenuState"));
	return true;
}
