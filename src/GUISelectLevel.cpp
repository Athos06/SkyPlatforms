#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"

#include "GameManager.h"
#include "GUIManager.h"
#include "GUISelectLevel.h"
#include "LevelsManager.h"
#include "AudioManager.h"
#include "Constants.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUISelectLevel* Ogre::Singleton<GUISelectLevel>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectLevel& GUISelectLevel::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectLevel* GUISelectLevel::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectLevel::GUISelectLevel(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISelectLevel::~GUISelectLevel(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectLevel::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectLevel::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetSelectLevel;	 


	
	CEGUI::Window* lvl1Button = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/Lvl1Button");
	lvl1Button->setEnabled(false);
	
	CEGUI::Window* lvl2Button = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/Lvl2Button");
	lvl2Button->setEnabled(false);

	CEGUI::Window* lvl3Button = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/Lvl3Button");
	lvl3Button->setEnabled(false);

	CEGUI::Window* lvlBossButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/LvlBossButton");
	lvlBossButton->setEnabled(false);

	if(GameConfig::getSingletonPtr()->levelsLock[0])
		lvl1Button->setEnabled(true);
	if(GameConfig::getSingletonPtr()->levelsLock[1])
		lvl2Button->setEnabled(true);
	if(GameConfig::getSingletonPtr()->levelsLock[2])	
		lvl3Button->setEnabled(true);
	if(GameConfig::getSingletonPtr()->levelsLock[3])	
		lvlBossButton->setEnabled(true);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectLevel::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISelectLevel::resume(){
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectLevel::onLvl1Button(const CEGUI::EventArgs &args){
	
	LevelsManager::getSingletonPtr()->selectLevel(
		LevelsManager::getSingletonPtr()->findByName("Level1"));
	
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("GameState"));

	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUISelectLevel::onLvl2Button(const CEGUI::EventArgs &args){
	LevelsManager::getSingletonPtr()->selectLevel(
		LevelsManager::getSingletonPtr()->findByName("Level2"));

	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("GameState"));
	
	return true;
}


bool GUISelectLevel::onLvl3Button(const CEGUI::EventArgs &args){
	LevelsManager::getSingletonPtr()->selectLevel(
		LevelsManager::getSingletonPtr()->findByName("Level3"));

	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("GameState"));
	
	return true;
}


bool GUISelectLevel::onLvlBossButton(const CEGUI::EventArgs &args){
	LevelsManager::getSingletonPtr()->selectLevel(
		LevelsManager::getSingletonPtr()->findByName("LevelBoss"));

	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("GameState"));
	
	return true;
}

bool GUISelectLevel::onBackButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	popGUIState();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||